#include "App.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Core/Event.hpp"
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
  m_window->setEventCallback([this](Event &e) { this->onEvent(e); });

  m_ourModel = std::make_shared<Model>(modelPath);
  m_transform.Position = glm::vec3(0);
  m_transform.Rotation = glm::vec3(0);
  m_transform.Scale = glm::vec3(1);

  m_projection = glm::perspective(
      glm::radians(45.f), (float)m_window->getWidth() / m_window->getHeight(),
      0.1f, 100.f);

  Renderer::init();
  Renderer::setClearColor({0.1f, 0.1f, 0.2f, 1.0f});
}

App::~App() { glfwTerminate(); }

void App::run() {
  auto shader = ResourceManager::loadShader(
      "default", "assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

  while (m_isRunning) {
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    processInput();

    Renderer::clear();

    shader->useShader();
    shader->setUniformVec3("lightPos", m_lightPos);
    shader->setUniformVec4("plane", m_plane);

    Renderer::beginScene(m_camera, m_projection, *shader);
    Renderer::submit(m_ourModel, m_transform, *shader);
    Renderer::endScene();

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwWindowShouldClose(m_window->getHandle())) {
    m_isRunning = false;
  }

  const float rotateSpeed = 90.0f * m_deltaTime;
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    m_transform.Rotation.y += rotateSpeed;
  }
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    m_transform.Rotation.y -= rotateSpeed;
  }
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_UP) == GLFW_PRESS) {
    m_transform.Rotation.x -= rotateSpeed;
  }
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_DOWN) == GLFW_PRESS) {
    m_transform.Rotation.x += rotateSpeed;
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
  if (height == 0)
    height = 1;

  m_projection = glm::perspective(glm::radians(45.f),
                                  (float)width / (float)height, 0.1f, 100.f);
}