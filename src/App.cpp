#include "App.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

App::App(std::filesystem::path modelPath) {
  if (!glfwInit()) {
    throw std::runtime_error("ERROR::GLFW::FAILED_TO_INIT_GLFW");
  }
  Window::init();

  m_window = std::make_unique<Window>();
  m_ourModel = std::make_unique<Model>(modelPath);
  m_transform.Position = glm::vec3(0);
  m_transform.Rotation = glm::vec3(0);
  m_transform.Scale = glm::vec3(1);

  m_projection = glm::perspective(
      glm::radians(45.f), (float)m_window->getWidth() / m_window->getHeight(),
      0.1f, 100.f);

  glEnable(GL_DEPTH_TEST);
}

App::~App() { glfwTerminate(); }

void App::run() {
  Shader shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
  glm::mat4 model, view, projection;

  while (m_isRunning) {
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    processInput();

    // Render
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = m_camera.getViewMatrix();
    model = m_transform.getModelMatrix();

    shader.setUniformMat4("model", model);
    shader.setUniformMat4("view", view);
    shader.setUniformMat4("projection", m_projection);
    shader.setUniformVec3("lightPos", m_lightPos);
    shader.setUniformVec3("viewPos", m_camera.getPosition());

    shader.useShader();

    m_ourModel->draw(shader);

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwWindowShouldClose(m_window->getHandle())) {
    m_isRunning = false;
  }

  const float lightSpeed = 2.5f * m_deltaTime;
  const float rotateSpeed = 90.0f * m_deltaTime;

  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_L) == GLFW_PRESS) {
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      m_lightPos.x += lightSpeed;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      m_lightPos.x -= lightSpeed;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_UP) == GLFW_PRESS) {
      m_lightPos.y -= lightSpeed;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      m_lightPos.y += lightSpeed;
    }
  } else {
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
}