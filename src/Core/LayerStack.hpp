#pragma once

#include "Core/Layer.hpp"
#include <algorithm>
#include <vector>

class LayerStack {
public:
  ~LayerStack() {
    for (Layer *layer : m_layers) {
      layer->onDetach();
      delete layer;
    }
  }

  void pushLayer(Layer *layer) {
    m_layers.emplace_back(layer);
    layer->onAttach();
  }

  void popLayer(Layer *layer) {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
      layer->onDetach();
      m_layers.erase(it);
    }
  }

  std::vector<Layer *>::iterator begin() { return m_layers.begin(); }
  std::vector<Layer *>::iterator end() { return m_layers.end(); }
  std::vector<Layer *>::reverse_iterator rbegin() { return m_layers.rbegin(); }
  std::vector<Layer *>::reverse_iterator rend() { return m_layers.rend(); }

private:
  std::vector<Layer *> m_layers;
};