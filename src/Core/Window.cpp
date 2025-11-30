#include "Core/Window.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Core/Event.hpp"

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
  glfwSetWindowUserPointer(m_nativeHandle, this);
  glfwMakeContextCurrent(m_nativeHandle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("ERROR::GLAD::FAILED_TO_INIT_GLAD");
  }

  glfwSetFramebufferSizeCallback(m_nativeHandle, [](GLFWwindow *window,
                                                    int width, int height) {
    auto *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (instance && instance->m_eventCallback) {
      WindowResizeEvent event(width, height);
      instance->m_eventCallback(event);
    }
  });
  glfwSetWindowCloseCallback(m_nativeHandle, [](GLFWwindow *window) {
    auto *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (instance && instance->m_eventCallback) {
      WindowCloseEvent event;
      instance->m_eventCallback(event);
    }
  });
}

Window::~Window() { glfwDestroyWindow(m_nativeHandle); };

Window::Window(Window &&other) noexcept {
  m_nativeHandle = std::exchange(other.m_nativeHandle, nullptr);
  m_properties.width = other.m_properties.width;
  m_properties.height = other.m_properties.height;
  m_properties.title = other.m_properties.title;
};
Window &Window::operator=(Window &&other) noexcept {
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
}

void Window::onUpdate() {
  glfwSwapBuffers(m_nativeHandle);
  glfwPollEvents();
};