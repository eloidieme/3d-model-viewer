#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "Core/Transform.hpp"
#include "Graphics/Camera.hpp"

class Event;
class Model;
class Window;

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
  glm::vec4 m_plane{0.5f, 0.5f, 0.0f, -0.5f};

  std::shared_ptr<Model> m_ourModel;
  std::unique_ptr<Window> m_window;

  void processInput();
  void onResize(int width, int height);
  void onEvent(Event &e);
};