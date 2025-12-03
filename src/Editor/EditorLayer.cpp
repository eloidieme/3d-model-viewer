#include "Editor/EditorLayer.hpp"

#include "Config.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/Log.hpp"
#include "Scene/Model.hpp"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

EditorLayer::EditorLayer(const std::string &modelPath)
    : m_modelPath(modelPath) {}

void EditorLayer::onAttach() {
  m_renderer.init();
  m_renderer.setClearColor(Config::Render::ClearColor);

  m_scene = std::make_unique<Scene>();

  m_scene->getCamera().setAspectRatio((float)Config::Window::Width,
                                      (float)Config::Window::Height);

  auto shader = m_resourceManager.loadShader(
      "default", Config::Paths::ShaderVert, Config::Paths::ShaderFrag);
  Model myModel(m_modelPath, m_resourceManager, shader);
  myModel.addToScene(*m_scene);

  std::vector<Vertex> vertices = {
      {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-Left
      {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Bottom-Right
      {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // Top-Right
      {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}   // Top-Left
  };
  std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
  m_planeMesh = std::make_shared<Mesh>(vertices, indices);

  auto planeShader = m_resourceManager.loadShader(
      "plane", Config::Paths::PlaneShaderVert, Config::Paths::PlaneShaderFrag);
  m_planeMaterial = std::make_shared<Material>(planeShader);
  m_planeMaterial->setVec4("u_Color", glm::vec4(0.8f, 0.8f, 0.8f, 0.1f));
  m_planeMaterial->setTransparent(true);

  LOG_CORE_INFO("Editor Layer Attached");
}

void EditorLayer::onDetach() {
  // Resource cleanup handled by destructors
}

void EditorLayer::onUpdate(float dt) {
  if (m_viewportFocused) {
    glm::vec2 delta = Input::getMouseDelta();
    m_scene->onMouseView(delta.x, delta.y);
    m_scene->onUpdate(dt);
  }

  m_renderer.clear();
  m_renderer.beginScene(*m_scene);

  auto &planes = m_scene->getClippingPlanes();
  for (auto &plane : planes) {
    glm::vec3 normal(plane.x, plane.y, plane.z);
    float dist = plane.w;

    glm::vec3 position = -normal * dist;
    glm::quat rotation =
        glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(normal));

    Transform t;
    t.setPosition(position);
    t.setScale(glm::vec3(5.0f));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model *= glm::toMat4(rotation);
    model = glm::scale(model, glm::vec3(5.0f));

    m_renderer.submit(m_planeMesh, m_planeMaterial, model);
  }

  m_renderer.endScene();
}

void EditorLayer::onImGuiRender() {
  ImGui::Begin("Settings");

  ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Separator();

  if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::DragFloat3("Light Position", &m_scene->getLightPos().x, 0.1f);

    if (ImGui::ColorEdit3("Clear Color", &Config::Render::ClearColor.x)) {
      m_renderer.setClearColor(Config::Render::ClearColor);
    }
  }

  if (ImGui::CollapsingHeader("Clipping Planes",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    auto &planes = m_scene->getClippingPlanes();

    if (planes.size() < 8) {
      if (ImGui::Button("Add Plane")) {
        planes.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
      }
    }

    for (int i = 0; i < planes.size(); i++) {
      ImGui::PushID(i);
      ImGui::Separator();
      ImGui::Text("Plane %d", i);

      ImGui::DragFloat3("Normal", &planes[i].x, 0.01f, -1.0f, 1.0f);

      ImGui::SameLine();
      if (ImGui::Button("Normalize")) {
        glm::vec3 normal(planes[i].x, planes[i].y, planes[i].z);
        if (glm::length(normal) > 0.0001f) {
          normal = glm::normalize(normal);
          planes[i].x = normal.x;
          planes[i].y = normal.y;
          planes[i].z = normal.z;
        }
      }

      ImGui::DragFloat("Distance", &planes[i].w, 0.1f);

      if (ImGui::Button("Remove")) {
        planes.erase(planes.begin() + i);
        i--;
      }
      ImGui::PopID();
    }
  }

  ImGui::End();
}

void EditorLayer::onEvent(Event &e) {
  if (e.getType() == EventType::WindowResize) {
    auto &resizeEvent = static_cast<WindowResizeEvent &>(e);
    if (resizeEvent.getWidth() > 0 && resizeEvent.getHeight() > 0) {
      m_scene->getCamera().setAspectRatio((float)resizeEvent.getWidth(),
                                          (float)resizeEvent.getHeight());
    }
  }

  if (e.getType() == EventType::KeyPressed) {
    onKeyPressed(static_cast<KeyPressedEvent &>(e));
  } else if (e.getType() == EventType::MouseButtonPressed) {
    onMouseButtonPressed(static_cast<MouseButtonPressedEvent &>(e));
  }
}

bool EditorLayer::onKeyPressed(KeyPressedEvent &e) {
  if (e.getKeyCode() == KeyCode::Tab) {
    if (m_viewportFocused) {
      Input::setCursorMode(CursorMode::Normal);
      m_viewportFocused = false;
    } else {
      Input::setCursorMode(CursorMode::Locked);
      m_viewportFocused = true;
    }
    return true;
  }

  if (e.getKeyCode() == KeyCode::R) {
    LOG_INFO("Reloading Shaders...");
    m_resourceManager.reloadAllShaders();
    return true;
  }

  return false;
}

bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent &e) {
  if (e.getMouseButton() == MouseCode::ButtonLeft) {
    if (!m_viewportFocused) {
      Input::setCursorMode(CursorMode::Locked);
      m_viewportFocused = true;
      return true;
    }
  }
  return false;
}