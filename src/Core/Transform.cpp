#include "Core/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
    : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), m_dirty(true),
      m_cachedMatrix(1.0f) {}

const glm::vec3 &Transform::getPosition() const { return m_position; }
const glm::vec3 &Transform::getRotation() const { return m_rotation; }
const glm::vec3 &Transform::getScale() const { return m_scale; }

void Transform::setPosition(const glm::vec3 &position) {
  m_position = position;
  m_dirty = true;
}

void Transform::setRotation(const glm::vec3 &rotation) {
  m_rotation = rotation;
  m_dirty = true;
}

void Transform::setScale(const glm::vec3 &scale) {
  m_scale = scale;
  m_dirty = true;
}

const glm::mat4 &Transform::getModelMatrix() const {
  if (m_dirty) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
    model = glm::rotate(model, glm::radians(m_rotation.x),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z),
                        glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_scale);

    m_cachedMatrix = model;
    m_dirty = false;
  }
  return m_cachedMatrix;
}