#pragma once

#include <memory>

#include "Window.hpp"

class App {
public:
  App();
  ~App();
  void run();

private:
  unsigned int VBO, VAO;

  bool m_isRunning = true;
  std::unique_ptr<Window> m_window;

  void processInput();
};