#include "Core/InputManager.hpp"
#include "Core/Input.hpp"

void InputManager::update() {
  m_prevKeyStates = m_keyStates;

  for (auto const &[action, key] : m_keyBindings) {
    m_keyStates[key] = Input::isKeyPressed(key);
  }
}

void InputManager::setBinding(Action action, KeyCode key) {
  m_keyBindings[action] = key;

  bool isPressed = Input::isKeyPressed(key);
  m_keyStates[key] = isPressed;
  m_prevKeyStates[key] = isPressed;
}

bool InputManager::isActionPressed(Action action) const {
  auto it = m_keyBindings.find(action);
  if (it == m_keyBindings.end())
    return false;

  KeyCode key = it->second;
  if (m_keyStates.find(key) != m_keyStates.end()) {
    return m_keyStates.at(key);
  }

  return false;
}

bool InputManager::isActionJustPressed(Action action) const {
  auto it = m_keyBindings.find(action);
  if (it == m_keyBindings.end())
    return false;

  KeyCode key = it->second;
  bool current = false;
  bool prev = false;

  if (m_keyStates.find(key) != m_keyStates.end())
    current = m_keyStates.at(key);
  if (m_prevKeyStates.find(key) != m_prevKeyStates.end())
    prev = m_prevKeyStates.at(key);

  return current && !prev;
}

bool InputManager::isActionJustReleased(Action action) const {
  auto it = m_keyBindings.find(action);
  if (it == m_keyBindings.end())
    return false;

  KeyCode key = it->second;
  bool current = false;
  bool prev = false;

  if (m_keyStates.find(key) != m_keyStates.end())
    current = m_keyStates.at(key);
  if (m_prevKeyStates.find(key) != m_prevKeyStates.end())
    prev = m_prevKeyStates.at(key);

  return !current && prev;
}