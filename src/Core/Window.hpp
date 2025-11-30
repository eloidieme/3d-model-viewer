#pragma once

#include "Config.hpp"

#include <functional>
#include <string>

class Event;

typedef struct GLFWwindow GLFWwindow;

struct WindowProps {
  unsigned int width;
  unsigned int height;
  std::string title;
};

class Window {
public:
  using EventCallbackFn = std::function<void(Event &)>;

  Window(unsigned int width, unsigned int height, const std::string &title);
  Window()
      : Window(Config::Window::Width, Config::Window::Height,
               Config::Window::Title) {};
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  Window(Window &&other) noexcept;
  Window &operator=(Window &&other) noexcept;

  static void init();
  void onUpdate();

  void setEventCallback(const EventCallbackFn &callback) {
    m_eventCallback = callback;
  }

  [[nodiscard]] GLFWwindow *getHandle() { return m_nativeHandle; }
  [[nodiscard]] unsigned int getWidth() { return m_properties.width; }
  [[nodiscard]] unsigned int getHeight() { return m_properties.height; }

private:
  GLFWwindow *m_nativeHandle;
  WindowProps m_properties;
  EventCallbackFn m_eventCallback;
};
