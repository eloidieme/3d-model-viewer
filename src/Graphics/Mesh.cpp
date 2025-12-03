#include "Graphics/Mesh.hpp"
#include "Graphics/GeometryManager.hpp"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices) {
  MeshRange range = GeometryManager::get().upload(vertices, indices);

  m_baseVertex = range.vertexOffset;
  m_indexOffset = range.indexOffset;
  m_indexCount = range.indexCount;
}

void Mesh::drawGeometry() const {
  glDrawElementsBaseVertex(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT,
                           (void *)(uintptr_t)m_indexOffset, m_baseVertex);
}