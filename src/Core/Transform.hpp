#pragma once

#include <glm/glm.hpp>

class Transform {
public:
  Transform();

  const glm::vec3 &getPosition() const;
  const glm::vec3 &getRotation() const;
  const glm::vec3 &getScale() const;

  void setPosition(const glm::vec3 &position);
  void setRotation(const glm::vec3 &rotation);
  void setScale(const glm::vec3 &scale);

  const glm::mat4 &getModelMatrix() const;

private:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;

  mutable bool m_dirty;
  mutable glm::mat4 m_cachedMatrix;
};