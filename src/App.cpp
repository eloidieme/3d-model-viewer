#include "App.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

App::App() {
  if (!glfwInit()) {
    throw std::runtime_error("ERROR::GLFW::FAILED_TO_INIT_GLFW");
  }
  Window::init();
  m_window = std::make_unique<Window>();

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f, // top
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

App::~App() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
}

void App::run() {
  Shader shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
  while (m_isRunning) {
    processInput();

    // Render
    glClearColor(0.5f, 0.0f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.useShader();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_window->onUpdate();
  }
}

void App::processInput() {
  if (glfwGetKey(m_window->getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwWindowShouldClose(m_window->getHandle())) {
    m_isRunning = false;
  }
}