#pragma once

#include "Core/Event.hpp"

class Layer {
public:
  virtual ~Layer() = default;

  virtual void onAttach() {}
  virtual void onDetach() {}

  virtual void onUpdate(float dt) {}
  virtual void onImGuiRender() {}
  virtual void onEvent(Event &event) {}
};