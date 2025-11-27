#pragma once

#include <memory>

#include "Model.hpp"
#include "Window.hpp"

class App {
public:
  App();
  ~App();
  void run();

private:
  unsigned int VBO, VAO;
  std::unique_ptr<Model> m_ourModel;

  bool m_isRunning = true;
  std::unique_ptr<Window> m_window;

  void processInput();
};