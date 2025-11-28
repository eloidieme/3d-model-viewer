#pragma once

#include "Camera.hpp"
#include "Model.hpp"
#include "Transform.hpp"
#include "Window.hpp"

#include <filesystem>
#include <memory>

class App {
public:
  App(std::filesystem::path modelPath);
  ~App();
  void run();

private:
  float m_deltaTime = 0.0f;
  float m_lastFrame = 0.0f;

  bool m_isRunning = true;
  struct Transform m_transform;
  Camera m_camera;

  glm::vec3 m_lightPos{2.0f, 2.0f, 2.0f};
  glm::mat4 m_projection;

  std::unique_ptr<Model> m_ourModel;
  std::unique_ptr<Window> m_window;

  void processInput();
};