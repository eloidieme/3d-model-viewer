#include "Window.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

Window::Window(unsigned int width, unsigned int height,
               const std::string &title) {
  m_properties.width = width;
  m_properties.height = height;
  m_properties.title = title;
  m_nativeHandle =
      glfwCreateWindow(m_properties.width, m_properties.height,
                       m_properties.title.c_str(), nullptr, nullptr);
  if (m_nativeHandle == nullptr) {
    throw std::runtime_error("ERROR::WINDOW::FAILED_TO_CREATE_WINDOW");
  }
  glfwMakeContextCurrent(m_nativeHandle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  glfwSetFramebufferSizeCallback(m_nativeHandle,
                                 Window::framebuffer_size_callback);
}

Window::~Window() { glfwDestroyWindow(m_nativeHandle); };

Window::Window(Window &&other) {
  m_nativeHandle = std::exchange(other.m_nativeHandle, nullptr);
  m_properties.width = other.m_properties.width;
  m_properties.height = other.m_properties.height;
  m_properties.title = other.m_properties.title;
};
Window &Window::operator=(Window &&other) {
  if (m_nativeHandle != nullptr) {
    glfwDestroyWindow(m_nativeHandle);
  }
  m_nativeHandle = std::exchange(other.m_nativeHandle, nullptr);
  m_properties.width = other.m_properties.width;
  m_properties.height = other.m_properties.height;
  m_properties.title = other.m_properties.title;

  return *this;
};

void Window::init() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Temporary
  glfwInit();
}

void Window::onUpdate() {
  glfwSwapBuffers(m_nativeHandle);
  glfwPollEvents();
};