#include "Core/Input.hpp"
#include "Core/Log.hpp"

#include <GLFW/glfw3.h>

GLFWwindow *Input::s_window = nullptr;

void Input::init(GLFWwindow *window) {
  s_window = window;
  LOG_CORE_INFO("Input system initialized");
}

bool Input::isKeyPressed(KeyCode keycode) {
  ASSERT(s_window, "Input::init() was not called before checking keys!");

  int state = glfwGetKey(s_window, static_cast<int>(keycode));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

std::pair<double, double> Input::getMousePosition() {
  ASSERT(s_window, "Input::init() was not called before checking mouse!");

  double x, y;
  glfwGetCursorPos(s_window, &x, &y);
  return {x, y};
}