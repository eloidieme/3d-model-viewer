#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices);
  ~Mesh() = default;

  void drawGeometry() const;

private:
  unsigned int m_baseVertex;
  unsigned int m_indexOffset;
  unsigned int m_indexCount;
};