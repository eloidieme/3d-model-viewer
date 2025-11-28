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
  bool m_isRunning = true;
  float m_rotX = 0.0f;
  float m_rotY = 0.0f;
  glm::vec3 m_lightPos{2.0f, 2.0f, 2.0f};
  glm::vec3 m_viewPos{0.0f, 0.0f, 7.5f};

  std::unique_ptr<Model> m_ourModel;
  std::unique_ptr<Window> m_window;

  void processInput();
};