#pragma once

#include "Graphics/Mesh.hpp"
#include <glad/glad.h>
#include <vector>

struct MeshRange {
  unsigned int vertexOffset;
  unsigned int indexOffset;
  unsigned int indexCount;
};

class GeometryManager {
public:
  static GeometryManager &get() {
    static GeometryManager instance;
    return instance;
  }

  void init();
  void shutdown();

  MeshRange upload(const std::vector<Vertex> &vertices,
                   const std::vector<unsigned int> &indices);

  unsigned int getGlobalVAO() const { return m_globalVAO; }
  unsigned int getGlobalBuffer() const { return m_globalBuffer; }

private:
  GeometryManager() = default;

  unsigned int m_globalBuffer = 0;
  unsigned int m_globalVAO = 0;

  size_t m_verticesHead = 0;
  size_t m_indicesHead = 0;

  const size_t MAX_VERTEX_MEMORY = 32 * 1024 * 1024;
  const size_t MAX_INDEX_MEMORY = 32 * 1024 * 1024;

  size_t m_indicesStartOffset = MAX_VERTEX_MEMORY;
};