#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
  Camera() {
    m_position = glm::vec3{0.0f, 0.0f, 7.5f};
    m_target = glm::vec3{0.0f, 0.0f, 0.0f};
    m_up = glm::vec3{0.0f, 1.0f, 0.0f};
  }
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
      : m_position(position), m_target(target), m_up(up) {}

  glm::mat4 getViewMatrix() { return glm::lookAt(m_position, m_target, m_up); }
  glm::vec3 getPosition() const { return m_position; }

private:
  glm::vec3 m_position;
  glm::vec3 m_target;
  glm::vec3 m_up;
};