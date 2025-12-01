#include "App.hpp"

#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Core/Event.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Scene/Model.hpp"

App::App(std::filesystem::path modelPath) {
  if (!glfwInit()) {
    throw std::runtime_error("ERROR::GLFW::FAILED_TO_INIT_GLFW");
  }
  Window::init();
  m_window = std::make_unique<Window>();
  glfwSetInputMode(m_window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Input::init(m_window->getHandle());
  m_window->setEventCallback([this](Event &e) { this->onEvent(e); });

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_window->getHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  m_renderer.init();
  m_renderer.setClearColor(Config::Render::ClearColor);

  m_ourModel = std::make_shared<Model>(modelPath, m_resourceManager);

  m_shader = m_resourceManager.loadShader("default", Config::Paths::ShaderVert,
                                          Config::Paths::ShaderFrag);
  m_planeShader = m_resourceManager.loadShader(
      "plane", Config::Paths::PlaneShaderVert, Config::Paths::PlaneShaderFrag);

  // Plane viz quad
  std::vector<Vertex> vertices = {
      {{-1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-Left
      {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Bottom-Right
      {{1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-Right
      {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}} // Top-Left
  };
  std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
  std::vector<std::shared_ptr<Texture>> textures;
  m_planeVisualizerMesh = std::make_unique<Mesh>(vertices, indices, textures);

  m_camera.setAspectRatio((float)m_window->getWidth(),
                          (float)m_window->getHeight());
}

App::~App() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  m_ourModel.reset();
  m_resourceManager.clear();
  m_window.reset();
  glfwTerminate();
}

void App::run() {
  while (m_isRunning) {
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    processInput();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_renderer.clear();

    m_shader->useShader();
    m_shader->setUniformVec3("lightPos", m_lightPos);

    m_shader->setUniformInt("u_ActiveClippingPlanes",
                            static_cast<int>(m_clippingPlanes.size()));
    for (size_t i = 0; i < m_clippingPlanes.size(); i++) {
      std::string uniformName = "u_ClippingPlanes[" + std::to_string(i) + "]";
      m_shader->setUniformVec4(uniformName, m_clippingPlanes[i]);
    }

    m_renderer.beginScene(m_camera, *m_shader);
    m_renderer.submit(m_ourModel, m_transform, *m_shader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    m_planeShader->useShader();

    m_planeShader->setUniformVec4("u_Color",
                                  glm::vec4(0.7f, 0.7f, 0.7f, 0.10f));

    for (const auto &plane : m_clippingPlanes) {
      glm::vec3 normal(plane.x, plane.y, plane.z);
      float dist = plane.w;

      glm::vec3 position = -normal * dist;
      glm::quat rotation =
          glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(normal));

      glm::vec3 scale(5.0f);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, position);
      model = model * glm::toMat4(rotation);
      model = glm::scale(model, scale);

      m_renderer.submit(*m_planeVisualizerMesh, model, *m_planeShader);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_renderer.endScene();

    ImGui::Begin("Settings");
    ImGui::Text("Application Stats");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    ImGui::Text("Render Settings");
    if (ImGui::ColorEdit3("Clear Color", &Config::Render::ClearColor.x)) {
      m_renderer.setClearColor(Config::Render::ClearColor);
    }
    ImGui::DragFloat3("Light Position", &m_lightPos.x, 0.1f);

    ImGui::Separator();
    ImGui::Text("Clipping Planes");

    if (m_clippingPlanes.size() < 8) {
      if (ImGui::Button("Add Clipping Plane")) {
        m_clippingPlanes.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
      }
    } else {
      ImGui::TextDisabled("Max clipping planes reached (8)");
    }

    for (size_t i = 0; i < m_clippingPlanes.size(); i++) {
      ImGui::PushID(static_cast<int>(i));

      if (ImGui::CollapsingHeader(("Plane " + std::to_string(i)).c_str(),
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Normal (XYZ)", &m_clippingPlanes[i].x, 0.01f, -1.0f,
                          1.0f);
        ImGui::DragFloat("Distance (W)", &m_clippingPlanes[i].w, 0.1f);

        if (ImGui::Button("Normalize")) {
          glm::vec3 normal(m_clippingPlanes[i]);
          if (glm::length(normal) > 0.0001f) {
            normal = glm::normalize(normal);
            m_clippingPlanes[i].x = normal.x;
            m_clippingPlanes[i].y = normal.y;
            m_clippingPlanes[i].z = normal.z;
          }
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove")) {
          m_clippingPlanes.erase(m_clippingPlanes.begin() + i);
          i--;
        }
      }
      ImGui::PopID();
    }

    ImGui::Separator();
    ImGui::Text("Camera");
    auto camPos = m_camera.getPosition();
    ImGui::Text("Pos: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (Input::isKeyPressed(KeyCode::Escape))
    m_isRunning = false;

  static bool isTabPressed = false;
  if (Input::isKeyPressed(KeyCode::Tab)) {
    if (!isTabPressed) {
      m_uiMode = !m_uiMode;

      if (m_uiMode) {
        glfwSetInputMode(m_window->getHandle(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
      } else {
        glfwSetInputMode(m_window->getHandle(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
        m_firstMouse = true;
      }
      isTabPressed = true;
    }
  } else {
    isTabPressed = false;
  }

  if (m_uiMode)
    return;

  if (Input::isKeyPressed(KeyCode::Up) || Input::isKeyPressed(KeyCode::W))
    m_camera.processKeyboard(CameraMovement::FORWARD, m_deltaTime);
  if (Input::isKeyPressed(KeyCode::Down) || Input::isKeyPressed(KeyCode::S))
    m_camera.processKeyboard(CameraMovement::BACKWARD, m_deltaTime);
  if (Input::isKeyPressed(KeyCode::Left) || Input::isKeyPressed(KeyCode::A))
    m_camera.processKeyboard(CameraMovement::LEFT, m_deltaTime);
  if (Input::isKeyPressed(KeyCode::Right) || Input::isKeyPressed(KeyCode::D))
    m_camera.processKeyboard(CameraMovement::RIGHT, m_deltaTime);

  // Hot Reloading Shaders
  static bool isRPressed = false;
  if (Input::isKeyPressed(KeyCode::R)) {
    if (!isRPressed) {
      std::cout << "Reloading Shaders..." << std::endl;

      m_resourceManager.clear();

      try {
        auto newShader = m_resourceManager.loadShader(
            "default", Config::Paths::ShaderVert, Config::Paths::ShaderFrag);

        m_shader = newShader;
        std::cout << "Shader compilation successful!" << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "SHADER RELOAD ERROR:\n" << e.what() << std::endl;
      }

      isRPressed = true;
    }
  } else {
    isRPressed = false;
  }

  auto [xpos, ypos] = Input::getMousePosition();

  if (m_firstMouse) {
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = false;
  }

  float xoffset = xpos - m_lastX;
  float yoffset = m_lastY - ypos;

  m_lastX = xpos;
  m_lastY = ypos;

  m_camera.processMouseMovement(xoffset, yoffset);
}

void App::onEvent(Event &e) {
  if (e.getType() == EventType::WindowResize) {
    auto &resizeEvent = static_cast<WindowResizeEvent &>(e);

    this->onResize(resizeEvent.getWidth(), resizeEvent.getHeight());
  }

  if (e.getType() == EventType::WindowClose) {
    m_isRunning = false;
  }
}

void App::onResize(int width, int height) {
  m_camera.setAspectRatio((float)width, (float)height);
}