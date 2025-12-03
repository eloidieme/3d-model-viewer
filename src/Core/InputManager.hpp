#pragma once

#include "Core/InputActions.hpp"
#include "Core/KeyCodes.hpp"
#include <unordered_map>

class InputManager {
public:
  InputManager() = default;

  void update();
  void setBinding(Action action, KeyCode key);

  bool isActionPressed(Action action) const;
  bool isActionJustPressed(Action action) const;
  bool isActionJustReleased(Action action) const;

private:
  std::unordered_map<Action, KeyCode> m_keyBindings;

  std::unordered_map<KeyCode, bool> m_keyStates;
  std::unordered_map<KeyCode, bool> m_prevKeyStates;
};