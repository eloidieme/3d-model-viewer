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
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  ~Mesh();

  void drawGeometry();

private:
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;

  unsigned int VAO, VBO, EBO;
  void setupMesh();
};