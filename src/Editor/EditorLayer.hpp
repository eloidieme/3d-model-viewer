#pragma once

#include "Config.hpp"
#include "Core/Event.hpp"
#include "Core/Layer.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Scene/Scene.hpp"

class EditorLayer : public Layer {
public:
  EditorLayer(const Config &config, const std::string &modelPath,
              InputManager &inputManager);
  virtual ~EditorLayer() = default;

  void onAttach() override;
  void onDetach() override;

  void onUpdate(float dt) override;
  void onImGuiRender() override;
  void onEvent(Event &e) override;

private:
  Config m_config;

  ResourceManager m_resourceManager;
  InputManager &m_inputManager;
  Renderer m_renderer;
  std::unique_ptr<Scene> m_scene;

  std::shared_ptr<Mesh> m_planeMesh;
  std::shared_ptr<Material> m_planeMaterial;

  std::string m_modelPath;
  bool m_viewportFocused = false;

  bool onMouseButtonPressed(MouseButtonPressedEvent &e);
};