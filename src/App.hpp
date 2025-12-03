#pragma once

#include "Core/Event.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Window.hpp"
#include "UI/ImGuiLayer.hpp"

#include <filesystem>
#include <memory>

class App {
public:
  App(std::filesystem::path modelPath);
  ~App();

  void run();
  void onEvent(Event &e);

private:
  std::unique_ptr<Window> m_window;
  bool m_isRunning = true;

  LayerStack m_layerStack;
  ImGuiLayer *m_imguiLayer;

  float m_lastFrameTime = 0.0f;
};