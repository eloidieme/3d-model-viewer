#include "Graphics/Mesh.hpp"

#include <glad/glad.h>

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<std::shared_ptr<Texture>> textures)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)),
      m_textures(std::move(textures)) {
  setupMesh();
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh(Mesh &&other) noexcept {
  m_vertices = std::move(other.m_vertices);
  m_indices = std::move(other.m_indices);
  m_textures = std::move(other.m_textures);

  VAO = other.VAO;
  VBO = other.VBO;
  EBO = other.EBO;

  other.VAO = 0;
  other.VBO = 0;
  other.EBO = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_textures = std::move(other.m_textures);

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;

    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
  }

  return *this;
}

void Mesh::draw(Shader &shader) {
  shader.useShader();

  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < m_textures.size(); i++) {
    TextureType type = m_textures[i]->getType();
    std::string number;
    std::string name;

    if (type == TextureType::Diffuse) {
      name = "texture_diffuse";
      number = std::to_string(diffuseNr++);
    } else if (type == TextureType::Specular) {
      name = "texture_specular";
      number = std::to_string(specularNr++);
    } else if (type == TextureType::Normal) {
      name = "texture_normal";
      number = std::to_string(normalNr++);
    } else if (type == TextureType::Height) {
      name = "texture_height";
      number = std::to_string(heightNr++);
    }

    shader.setUniformInt((name + number).c_str(), i);

    m_textures[i]->bind(i);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
               &m_vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
               &m_indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}