#pragma once

#include "Core/KeyCodes.hpp"
#include <utility>

struct GLFWwindow;

class Input {
public:
  static void init(GLFWwindow *window);

  static bool isKeyPressed(KeyCode keycode);
  static std::pair<double, double> getMousePosition();

private:
  static GLFWwindow *s_window;
};