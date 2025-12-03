#pragma once

#include "Core/Event.hpp"
#include "Core/Layer.hpp"

struct GLFWwindow;

class ImGuiLayer : public Layer {
public:
  ImGuiLayer(GLFWwindow *windowHandle);
  ~ImGuiLayer() = default;

  void onAttach() override;
  void onDetach() override;
  void onEvent(Event &e) override;

  void begin();
  void end();

private:
  GLFWwindow *m_windowHandle;
};