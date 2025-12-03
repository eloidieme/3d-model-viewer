#include "Graphics/GeometryManager.hpp"
#include "Core/Log.hpp"

void GeometryManager::init() {
  glCreateBuffers(1, &m_globalBuffer);

  size_t totalSize = MAX_VERTEX_MEMORY + MAX_INDEX_MEMORY;
  glNamedBufferStorage(m_globalBuffer, totalSize, nullptr,
                       GL_DYNAMIC_STORAGE_BIT);

  glCreateVertexArrays(1, &m_globalVAO);

  glVertexArrayVertexBuffer(m_globalVAO, 0, m_globalBuffer, 0, sizeof(Vertex));

  glVertexArrayElementBuffer(m_globalVAO, m_globalBuffer);

  glEnableVertexArrayAttrib(m_globalVAO, 0);
  glVertexArrayAttribFormat(m_globalVAO, 0, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, Position));
  glVertexArrayAttribBinding(m_globalVAO, 0, 0);

  glEnableVertexArrayAttrib(m_globalVAO, 1);
  glVertexArrayAttribFormat(m_globalVAO, 1, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, Normal));
  glVertexArrayAttribBinding(m_globalVAO, 1, 0);

  glEnableVertexArrayAttrib(m_globalVAO, 2);
  glVertexArrayAttribFormat(m_globalVAO, 2, 2, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, TexCoords));
  glVertexArrayAttribBinding(m_globalVAO, 2, 0);

  LOG_CORE_INFO("GeometryManager initialized. Global Buffer: {0}MB",
                totalSize / 1024 / 1024);
}

MeshRange GeometryManager::upload(const std::vector<Vertex> &vertices,
                                  const std::vector<unsigned int> &indices) {
  size_t vertSize = vertices.size() * sizeof(Vertex);
  size_t idxSize = indices.size() * sizeof(unsigned int);

  if (m_verticesHead + vertSize > MAX_VERTEX_MEMORY) {
    LOG_CORE_ERROR("GeometryManager::upload - Vertex Buffer Overflow!");
    return {};
  }
  if (m_indicesHead + idxSize > MAX_INDEX_MEMORY) {
    LOG_CORE_ERROR("GeometryManager::upload - Index Buffer Overflow!");
    return {};
  }

  MeshRange range;

  range.vertexOffset =
      static_cast<unsigned int>(m_verticesHead / sizeof(Vertex));

  range.indexOffset =
      static_cast<unsigned int>(m_indicesStartOffset + m_indicesHead);

  range.indexCount = static_cast<unsigned int>(indices.size());

  glNamedBufferSubData(m_globalBuffer, m_verticesHead, vertSize,
                       vertices.data());

  glNamedBufferSubData(m_globalBuffer, m_indicesStartOffset + m_indicesHead,
                       idxSize, indices.data());

  m_verticesHead += vertSize;
  m_indicesHead += idxSize;

  return range;
}

void GeometryManager::shutdown() {
  glDeleteBuffers(1, &m_globalBuffer);
  glDeleteVertexArrays(1, &m_globalVAO);
}