#include "App.hpp"

#include <iostream>

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

  m_renderer.init();
  m_renderer.setClearColor(Config::Render::ClearColor);

  m_ourModel = std::make_shared<Model>(modelPath, m_resourceManager);

  m_shader = m_resourceManager.loadShader("default", Config::Paths::ShaderVert,
                                          Config::Paths::ShaderFrag);

  m_camera.setAspectRatio((float)m_window->getWidth(),
                          (float)m_window->getHeight());
}

App::~App() {
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

    m_renderer.clear();

    m_shader->useShader();
    m_shader->setUniformVec3("lightPos", m_lightPos);
    m_shader->setUniformVec4("plane", m_plane);

    m_renderer.beginScene(m_camera, *m_shader);
    m_renderer.submit(m_ourModel, m_transform, *m_shader);
    m_renderer.endScene();

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (Input::isKeyPressed(Key::Escape))
    m_isRunning = false;

  if (Input::isKeyPressed(Key::Up) || Input::isKeyPressed(Key::W))
    m_camera.processKeyboard(FORWARD, m_deltaTime);
  if (Input::isKeyPressed(Key::Down) || Input::isKeyPressed(Key::S))
    m_camera.processKeyboard(BACKWARD, m_deltaTime);
  if (Input::isKeyPressed(Key::Left) || Input::isKeyPressed(Key::A))
    m_camera.processKeyboard(LEFT, m_deltaTime);
  if (Input::isKeyPressed(Key::Right) || Input::isKeyPressed(Key::D))
    m_camera.processKeyboard(RIGHT, m_deltaTime);

  // Hot Reloading Shaders
  static bool isRPressed = false;
  if (Input::isKeyPressed(Key::R)) {
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