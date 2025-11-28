#include "App.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

App::App() {
  if (!glfwInit()) {
    throw std::runtime_error("ERROR::GLFW::FAILED_TO_INIT_GLFW");
  }
  Window::init();

  m_window = std::make_unique<Window>();
  m_ourModel = std::make_unique<Model>("assets/models/backpack/backpack.obj");

  glEnable(GL_DEPTH_TEST);
}

App::~App() { glfwTerminate(); }

void App::run() {
  Shader shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
  glm::mat4 model, view, projection;

  while (m_isRunning) {
    processInput();

    // Render
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projection = glm::perspective(
        glm::radians(45.f), (float)m_window->getWidth() / m_window->getHeight(),
        0.1f, 100.f);

    view = glm::translate(glm::mat4(1.0f), -m_viewPos);

    model = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotX),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model =
        glm::rotate(model, glm::radians(m_rotY), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.setUniformMat4("model", model);
    shader.setUniformMat4("view", view);
    shader.setUniformMat4("projection", projection);

    shader.useShader();
    shader.setUniformVec3("lightPos", m_lightPos);
    shader.setUniformVec3("viewPos", m_viewPos);

    m_ourModel->draw(shader);

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwWindowShouldClose(m_window->getHandle())) {
    m_isRunning = false;
  }

  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_L) == GLFW_PRESS) {
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      m_lightPos.x += 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      m_lightPos.x -= 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_UP) == GLFW_PRESS) {
      m_lightPos.y -= 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      m_lightPos.y += 1.0f;
    }
  } else {
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      m_rotY += 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      m_rotY -= 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_UP) == GLFW_PRESS) {
      m_rotX -= 1.0f;
    }
    if (glfwGetKey(m_window->getHandle(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      m_rotX += 1.0f;
    }
  }
}