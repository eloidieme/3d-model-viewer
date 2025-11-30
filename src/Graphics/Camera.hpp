#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 7.5f),
         glm::vec3 target = glm::vec3(0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
      : m_position(position), m_target(target), m_up(up) {
    updateProjection();
  }

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(m_position, m_target, m_up);
  }
  glm::vec3 getPosition() const { return m_position; }

  const glm::mat4 getProjectionMatrix() const { return m_projection; }

  void setAspectRatio(float width, float height) {
    if (height == 0)
      height = 1;
    m_aspectRatio = width / height;
    updateProjection();
  }

  void setFov(float fovInDegrees) {
    m_fov = fovInDegrees;
    updateProjection();
  }

private:
  glm::vec3 m_position;
  glm::vec3 m_target;
  glm::vec3 m_up;

  float m_fov = 45.0f;
  float m_aspectRatio = 1.33f;
  float m_near = 0.1f;
  float m_far = 100.f;

  glm::mat4 m_projection;

  void updateProjection() {
    m_projection =
        glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
  }
};