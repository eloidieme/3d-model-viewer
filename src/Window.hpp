#pragma once

#include <iostream>
#include <string>

typedef struct GLFWwindow GLFWwindow;

struct WindowProps {
  unsigned int width;
  unsigned int height;
  std::string title;
};

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string &title);
  Window() : Window(800, 600, "Window") {};
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  Window(Window &&other);
  Window &operator=(Window &&other);

  static void init();
  void onUpdate();

  GLFWwindow *getHandle() { return m_nativeHandle; }
  unsigned int getWidth() { return m_properties.width; }
  unsigned int getHeight() { return m_properties.height; }

private:
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  GLFWwindow *m_nativeHandle;
  WindowProps m_properties;
};
