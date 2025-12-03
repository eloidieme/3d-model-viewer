#pragma once

#include "Config.hpp"
#include "Core/Event.hpp"

#include <functional>
#include <string>

struct GLFWwindow;

struct WindowProps {
  unsigned int width;
  unsigned int height;
  std::string title;
};

class Window {
public:
  using EventCallbackFn = std::function<void(Event &)>;

  Window();
  ~Window();

  static void init();
  void onUpdate();

  void setEventCallback(const EventCallbackFn &callback) {
    m_data.EventCallback = callback;
  }

  [[nodiscard]] GLFWwindow *getHandle() { return m_nativeHandle; }
  [[nodiscard]] unsigned int getWidth() { return m_data.Width; }
  [[nodiscard]] unsigned int getHeight() { return m_data.Height; }

private:
  GLFWwindow *m_nativeHandle;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    EventCallbackFn EventCallback;
  };

  WindowData m_data;
};