#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "Config.hpp"
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

  float m_lastX = static_cast<float>(Config::Window::Width) / 2.0f;
  float m_lastY = static_cast<float>(Config::Window::Height) / 2.0f;
  bool m_firstMouse = true;

  bool m_isRunning = true;

  struct Transform m_transform;
  Camera m_camera;

  glm::vec3 m_lightPos{Config::Render::LightPosition};
  glm::vec4 m_plane{Config::Render::ClippingPlane};

  std::unique_ptr<Window> m_window;
  std::shared_ptr<Model> m_ourModel;

  void processInput();
  void onResize(int width, int height);
  void onEvent(Event &e);
};