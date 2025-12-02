#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
  Transform() : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f) {}

  const glm::vec3 &getPosition() const { return m_position; }
  const glm::vec3 &getRotation() const { return m_rotation; }
  const glm::vec3 &getScale() const { return m_scale; }

  void setPosition(const glm::vec3 &position) {
    m_position = position;
    m_dirty = true;
  }

  void setRotation(const glm::vec3 &rotation) {
    m_rotation = rotation;
    m_dirty = true;
  }

  void setScale(const glm::vec3 &scale) {
    m_scale = scale;
    m_dirty = true;
  }

  const glm::mat4 &getModelMatrix() const {
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

private:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;

  mutable bool m_dirty = true;
  mutable glm::mat4 m_cachedMatrix{1.0f};
};
