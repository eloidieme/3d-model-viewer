#include "App.hpp"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

#include "Core/Event.hpp"
#include "Core/Input.hpp"
#include "Core/Log.hpp"

App::App(std::filesystem::path modelPath) {
  if (!glfwInit())
    throw std::runtime_error("ERROR::GLFW::FAILED_TO_INIT_GLFW");
  Window::init();
  m_window = std::make_unique<Window>();

  Input::init(m_window->getHandle());
  m_window->setEventCallback([this](Event &e) { this->onEvent(e); });
  glfwSetInputMode(m_window->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(m_window->getHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  m_renderer.init();
  m_renderer.setClearColor(Config::Render::ClearColor);

  m_scene = std::make_unique<Scene>();
  m_scene->getCamera().setAspectRatio((float)m_window->getWidth(),
                                      (float)m_window->getHeight());

  auto shader = m_resourceManager.loadShader(
      "default", Config::Paths::ShaderVert, Config::Paths::ShaderFrag);

  Model myModel(modelPath.string(), m_resourceManager, shader);

  Transform transform;
  transform.setScale(glm::vec3(1.0f));
  myModel.addToScene(*m_scene, transform);
}

App::~App() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

void App::run() {
  while (m_isRunning) {
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    processInput();

    if (!m_uiMode) {
      m_scene->onUpdate(m_deltaTime);
    }

    m_renderer.clear();
    m_renderer.beginScene(*m_scene);
    m_renderer.endScene();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::DragFloat3("Light Pos", &m_scene->getLightPos().x, 0.1f);

    if (ImGui::ColorEdit3("Clear Color", &Config::Render::ClearColor.x)) {
      m_renderer.setClearColor(Config::Render::ClearColor);
    }

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
      glfwSetInputMode(m_window->getHandle(), GLFW_CURSOR,
                       m_uiMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
    isTabPressed = true;
  } else {
    isTabPressed = false;
  }
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
  m_scene->getCamera().setAspectRatio((float)width, (float)height);
}