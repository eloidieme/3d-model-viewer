#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "Config.hpp"
#include "Core/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Scene/Model.hpp"
#include "Scene/Scene.hpp"

class App {
public:
  App(std::filesystem::path modelPath);
  ~App();
  void run();

private:
  void processInput();
  void onResize(int width, int height);
  void onEvent(Event &e);

  // Core Systems
  std::unique_ptr<Window> m_window;
  ResourceManager m_resourceManager;
  Renderer m_renderer;

  std::unique_ptr<Scene> m_scene;

  // App state
  bool m_isRunning = true;
  bool m_uiMode = false;
  float m_deltaTime = 0.0f;
  float m_lastFrame = 0.0f;
};