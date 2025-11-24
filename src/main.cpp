#include "Window.hpp"
#include <GLFW/glfw3.h>

#include <iostream>

const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  Window::init();

  Window window(WIN_WIDTH, WIN_HEIGHT, "Window");

  while (!glfwWindowShouldClose(window.getHandle())) {
    processInput(window.getHandle());

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window.getHandle());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}