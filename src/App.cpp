#include "App.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

  m_ourModel = std::make_shared<Model>(modelPath);
  m_transform.Position = glm::vec3(0);
  m_transform.Rotation = glm::vec3(0);
  m_transform.Scale = glm::vec3(1);

  m_camera.setAspectRatio((float)m_window->getWidth(),
                          (float)m_window->getHeight());

  Renderer::init();
  Renderer::setClearColor({0.1f, 0.1f, 0.2f, 1.0f});
}

App::~App() {
  m_ourModel.reset();
  ResourceManager::clear();
  m_window.reset();
  glfwTerminate();
}

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

    Renderer::beginScene(m_camera, *shader);
    Renderer::submit(m_ourModel, m_transform, *shader);
    Renderer::endScene();

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (Input::isKeyPressed(Key::Escape))
    m_isRunning = false;

  if (Input::isKeyPressed(Key::Up) || Input::isKeyPressed(Key::Z))
    m_camera.processKeyboard(FORWARD, m_deltaTime);
  if (Input::isKeyPressed(Key::Down) || Input::isKeyPressed(Key::S))
    m_camera.processKeyboard(BACKWARD, m_deltaTime);
  if (Input::isKeyPressed(Key::Left) || Input::isKeyPressed(Key::Q))
    m_camera.processKeyboard(LEFT, m_deltaTime);
  if (Input::isKeyPressed(Key::Right) || Input::isKeyPressed(Key::D))
    m_camera.processKeyboard(RIGHT, m_deltaTime);

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