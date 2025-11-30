#pragma once

#include <utility>

struct GLFWwindow;

class Input {
public:
  static void init(GLFWwindow *window);

  static bool isKeyPressed(int keycode);
  static std::pair<double, double> getMousePosition();

private:
  static GLFWwindow *s_window;
};