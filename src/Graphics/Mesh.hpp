#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

class Shader;
class Texture;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<std::shared_ptr<Texture>> textures);
  ~Mesh();

  Mesh(const Mesh &other) = delete;
  Mesh &operator=(const Mesh &other) = delete;

  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;

  void draw(Shader &shader);

private:
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<std::shared_ptr<Texture>> m_textures;

  unsigned int VAO, VBO, EBO;

  void setupMesh();
};