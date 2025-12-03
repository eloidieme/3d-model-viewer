#include "App.hpp"
#include "Core/Input.hpp"
#include "Core/Log.hpp"
#include "Editor/EditorLayer.hpp"

#include <GLFW/glfw3.h>

App::App(std::filesystem::path modelPath) {
  if (!glfwInit())
    throw std::runtime_error("Failed to init GLFW");

  Window::init();
  m_window = std::make_unique<Window>();

  m_window->setEventCallback([this](Event &e) { this->onEvent(e); });

  // --- FIX 1: INITIALIZE INPUT SYSTEM ---
  Input::init(m_window->getHandle());
  // --------------------------------------

  m_layerStack.pushLayer(new EditorLayer(modelPath.string()));

  m_imguiLayer = new ImGuiLayer(m_window->getHandle());
  m_layerStack.pushLayer(m_imguiLayer);
}

App::~App() { glfwTerminate(); }

void App::run() {
  while (m_isRunning) {
    float time = (float)glfwGetTime();
    float dt = time - m_lastFrameTime;
    m_lastFrameTime = time;

    // --- FIX 2: UPDATE INPUT STATE ---
    // This calculates mouse delta for the frame
    Input::update();
    // ---------------------------------

    for (Layer *layer : m_layerStack) {
      layer->onUpdate(dt);
    }

    m_imguiLayer->begin();
    for (Layer *layer : m_layerStack) {
      layer->onImGuiRender();
    }
    m_imguiLayer->end();

    m_window->onUpdate();
  }
}

void App::onEvent(Event &e) {
  if (e.getType() == EventType::WindowClose) {
    m_isRunning = false;
    return;
  }

  for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
    (*it)->onEvent(e);
    if (e.m_handled) {
      break;
    }
  }
}