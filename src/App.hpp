#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "Config.hpp"
#include "Core/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Graphics/Shader.hpp"

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
  bool m_uiMode = false;

  struct Transform m_transform;
  Camera m_camera;

  glm::vec3 m_lightPos{Config::Render::LightPosition};
  std::vector<glm::vec4> m_clippingPlanes;

  std::unique_ptr<Window> m_window;
  ResourceManager m_resourceManager;
  Renderer m_renderer;

  std::shared_ptr<Model> m_ourModel;
  std::shared_ptr<Shader> m_shader;

  std::unique_ptr<Mesh> m_planeVisualizerMesh;
  std::shared_ptr<Shader> m_planeShader;

  void processInput();
  void onResize(int width, int height);
  void onEvent(Event &e);
};