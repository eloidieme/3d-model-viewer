#include "Core/Input.hpp"
#include "Core/Log.hpp"
#include <GLFW/glfw3.h>

GLFWwindow *Input::s_window = nullptr;
glm::vec2 Input::s_mousePos = {0.0f, 0.0f};
glm::vec2 Input::s_mouseDelta = {0.0f, 0.0f};
bool Input::s_firstMouse = true;

void Input::init(GLFWwindow *window) {
  s_window = window;
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  s_mousePos = {(float)x, (float)y};
  LOG_CORE_INFO("Input system initialized");
}

void Input::update() {
  if (!s_window)
    return;

  double x, y;
  glfwGetCursorPos(s_window, &x, &y);
  glm::vec2 currentPos = {(float)x, (float)y};

  if (s_firstMouse) {
    s_mousePos = currentPos;
    s_firstMouse = false;
  }

  s_mouseDelta = currentPos - s_mousePos;
  s_mouseDelta.y = -s_mouseDelta.y;
  s_mousePos = currentPos;
}

bool Input::isKeyPressed(KeyCode keycode) {
  return glfwGetKey(s_window, static_cast<int>(keycode)) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(MouseCode button) {
  return glfwGetMouseButton(s_window, static_cast<int>(button)) == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() { return s_mousePos; }
glm::vec2 Input::getMouseDelta() { return s_mouseDelta; }

void Input::setCursorMode(CursorMode mode) {
  if (!s_window)
    return;

  int glfwMode = GLFW_CURSOR_NORMAL;
  if (mode == CursorMode::Locked)
    glfwMode = GLFW_CURSOR_DISABLED;
  else if (mode == CursorMode::Hidden)
    glfwMode = GLFW_CURSOR_HIDDEN;

  glfwSetInputMode(s_window, GLFW_CURSOR, glfwMode);

  if (mode == CursorMode::Locked) {
    s_firstMouse = true;
    s_mouseDelta = {0.0f, 0.0f};
  }
}