#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  ~Mesh();

  Mesh(const Mesh &other) = delete;
  Mesh &operator=(const Mesh &other) = delete;

  Mesh(Mesh &&other);
  Mesh &operator=(Mesh &&other);

  void draw(Shader &shader);

private:
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<Texture> m_textures;

  unsigned int VAO, VBO, EBO;

  void setupMesh();
};