#pragma once

#include "Core/KeyCodes.hpp"
#include <sstream>
#include <string>

enum EventCategory {
  None = 0,
  EventCategoryApplication = 1 << 0,
  EventCategoryInput = 1 << 1,
  EventCategoryKeyboard = 1 << 2,
  EventCategoryMouse = 1 << 3,
  EventCategoryMouseButton = 1 << 4,
};

enum class EventType {
  None = 0,
  WindowResize,
  WindowClose,
  KeyPressed,
  KeyReleased,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled,
};

class Event {
public:
  virtual ~Event() = default;

  bool m_handled = false;

  virtual EventType getType() const = 0;
  virtual int getCategoryFlags() const = 0;

  bool isInCategory(EventCategory category) {
    return getCategoryFlags() & category;
  }
};

// --- Keyboard Events ---
class KeyEvent : public Event {
public:
  KeyCode getKeyCode() const { return m_keycode; }
  int getCategoryFlags() const override {
    return EventCategoryKeyboard | EventCategoryInput;
  }

protected:
  KeyEvent(KeyCode keycode) : m_keycode(keycode) {}
  KeyCode m_keycode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(KeyCode keycode, bool isRepeat = false)
      : KeyEvent(keycode), m_isRepeat(isRepeat) {}

  bool isRepeat() const { return m_isRepeat; }

  EventType getType() const override { return EventType::KeyPressed; }

private:
  bool m_isRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}
  EventType getType() const override { return EventType::KeyReleased; }
};

// --- Mouse Events ---
class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

  float getX() const { return m_mouseX; }
  float getY() const { return m_mouseY; }

  EventType getType() const override { return EventType::MouseMoved; }
  int getCategoryFlags() const override {
    return EventCategoryMouse | EventCategoryInput;
  }

private:
  float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : m_xOffset(xOffset), m_yOffset(yOffset) {}

  float getXOffset() const { return m_xOffset; }
  float getYOffset() const { return m_yOffset; }

  EventType getType() const override { return EventType::MouseScrolled; }
  int getCategoryFlags() const override {
    return EventCategoryMouse | EventCategoryInput;
  }

private:
  float m_xOffset, m_yOffset;
};

class MouseButtonEvent : public Event {
public:
  MouseCode getMouseButton() const { return m_button; }
  int getCategoryFlags() const override {
    return EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton;
  }

protected:
  MouseButtonEvent(MouseCode button) : m_button(button) {}
  MouseCode m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}
  EventType getType() const override { return EventType::MouseButtonPressed; }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}
  EventType getType() const override { return EventType::MouseButtonReleased; }
};

// --- Window Events ---
class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }

  EventType getType() const override { return EventType::WindowResize; }
  int getCategoryFlags() const override { return EventCategoryApplication; }

private:
  int m_width, m_height;
};

class WindowCloseEvent : public Event {
public:
  EventType getType() const override { return EventType::WindowClose; }
  int getCategoryFlags() const override { return EventCategoryApplication; }
};