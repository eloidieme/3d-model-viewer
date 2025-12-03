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
  glCreateVertexArrays(1, &VAO);
  glCreateBuffers(1, &VBO);
  glCreateBuffers(1, &EBO);

  glNamedBufferData(VBO, m_vertices.size() * sizeof(Vertex), m_vertices.data(),
                    GL_STATIC_DRAW);
  glNamedBufferData(EBO, m_indices.size() * sizeof(unsigned int),
                    m_indices.data(), GL_STATIC_DRAW);

  glVertexArrayElementBuffer(VAO, EBO);
  glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));

  glEnableVertexArrayAttrib(VAO, 0);
  glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, Position));
  glVertexArrayAttribBinding(VAO, 0, 0);

  glEnableVertexArrayAttrib(VAO, 1);
  glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, Normal));
  glVertexArrayAttribBinding(VAO, 1, 0);

  glEnableVertexArrayAttrib(VAO, 2);
  glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, TexCoords));
  glVertexArrayAttribBinding(VAO, 2, 0);
}