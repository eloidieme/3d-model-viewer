#pragma once

#include "Core/KeyCodes.hpp"
#include <glm/glm.hpp>

struct GLFWwindow;

enum class CursorMode { Normal = 0, Hidden = 1, Locked = 2 };

class Input {
public:
  static void init(GLFWwindow *window);

  static void update();

  static bool isKeyPressed(KeyCode keycode);

  static bool isMouseButtonPressed(MouseCode button);

  static glm::vec2 getMousePosition();

  static glm::vec2 getMouseDelta();
  static void setCursorMode(CursorMode mode);

private:
  static GLFWwindow *s_window;
  static glm::vec2 s_mousePos;
  static glm::vec2 s_mouseDelta;
  static bool s_firstMouse;
};