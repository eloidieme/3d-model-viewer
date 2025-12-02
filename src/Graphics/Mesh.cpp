#include "Graphics/Mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {
  setupMesh();
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}

void Mesh::drawGeometry() {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()),
                 GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
               &m_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
               &m_indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}