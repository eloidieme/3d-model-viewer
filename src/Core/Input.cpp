#include "Core/Input.hpp"
#include <GLFW/glfw3.h>

GLFWwindow *Input::s_window = nullptr;

void Input::init(GLFWwindow *window) { s_window = window; }

bool Input::isKeyPressed(int keycode) {
  if (!s_window)
    return false;

  int state = glfwGetKey(s_window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}