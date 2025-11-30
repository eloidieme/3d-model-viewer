#pragma once

enum class EventType {
  None = 0,
  WindowResize,
  WindowClose,
};

class Event {
public:
  virtual ~Event() = default;
  virtual EventType getType() const = 0;
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }

  EventType getType() const override { return EventType::WindowResize; }

private:
  int m_width, m_height;
};

class WindowCloseEvent : public Event {
public:
  EventType getType() const override { return EventType::WindowClose; }
};