#include "Core/Window.hpp"
#include "Core/Event.hpp"
#include "Core/Log.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stdexcept>

static void APIENTRY OpenGLMessageCallback(GLenum source, GLenum type,
                                           unsigned int id, GLenum severity,
                                           GLsizei length, const char *message,
                                           const void *userParam) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    LOG_CORE_CRITICAL("[OpenGL Debug HIGH] {0}", message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    LOG_CORE_ERROR("[OpenGL Debug MEDIUM] {0}", message);
    return;
  case GL_DEBUG_SEVERITY_LOW:
    LOG_CORE_WARN("[OpenGL Debug LOW] {0}", message);
    return;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    LOG_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
    return;
  }

  LOG_CORE_INFO("[OpenGL Debug UNKNOWN] {0}", message);
}

Window::Window(const WindowConfig &config) {
  m_data.Title = config.Title;
  m_data.Width = config.Width;
  m_data.Height = config.Height;

  LOG_CORE_INFO("Creating Window {0} ({1}x{2})", m_data.Title, m_data.Width,
                m_data.Height);

  m_nativeHandle = glfwCreateWindow(m_data.Width, m_data.Height,
                                    m_data.Title.c_str(), nullptr, nullptr);

  if (!m_nativeHandle) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_nativeHandle);
  glfwSetWindowUserPointer(m_nativeHandle, &m_data);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  LOG_CORE_INFO("OpenGL Info:");
  LOG_CORE_INFO("  Vendor: {0}", (const char *)glGetString(GL_VENDOR));
  LOG_CORE_INFO("  Renderer: {0}", (const char *)glGetString(GL_RENDERER));
  LOG_CORE_INFO("  Version: {0}", (const char *)glGetString(GL_VERSION));

#ifdef DEBUG
  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    LOG_CORE_INFO("OpenGL Debug Output Enabled");
  }
#endif

  glfwSetFramebufferSizeCallback(
      m_nativeHandle, [](GLFWwindow *window, int width, int height) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetWindowCloseCallback(m_nativeHandle, [](GLFWwindow *window) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetKeyCallback(m_nativeHandle, [](GLFWwindow *window, int key,
                                        int scancode, int action, int mods) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    switch (action) {
    case GLFW_PRESS: {
      KeyPressedEvent event((KeyCode)key, false);
      data.EventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent event((KeyCode)key);
      data.EventCallback(event);
      break;
    }
    case GLFW_REPEAT: {
      KeyPressedEvent event((KeyCode)key, true);
      data.EventCallback(event);
      break;
    }
    }
  });

  glfwSetCharCallback(
      m_nativeHandle, [](GLFWwindow *window, unsigned int keycode) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

        KeyTypedEvent event((KeyCode)keycode);
        data.EventCallback(event);
      });

  glfwSetMouseButtonCallback(
      m_nativeHandle, [](GLFWwindow *window, int button, int action, int mods) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event((MouseCode)button);
          data.EventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event((MouseCode)button);
          data.EventCallback(event);
          break;
        }
        }
      });

  glfwSetScrollCallback(
      m_nativeHandle, [](GLFWwindow *window, double xOffset, double yOffset) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
      });

  glfwSetCursorPosCallback(
      m_nativeHandle, [](GLFWwindow *window, double xPos, double yPos) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
      });
}

Window::~Window() { glfwDestroyWindow(m_nativeHandle); };

void Window::init() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}

void Window::onUpdate() {
  glfwSwapBuffers(m_nativeHandle);
  glfwPollEvents();
};