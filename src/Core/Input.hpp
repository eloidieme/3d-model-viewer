#pragma once

struct GLFWwindow;

class Input {
public:
  static void init(GLFWwindow *window);

  static bool isKeyPressed(int keycode);

private:
  static GLFWwindow *s_window;
};