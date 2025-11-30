#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {

public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
         float pitch = 0.0f)
      : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(2.5f),
        m_mouseSensitivity(0.1f) {
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
  }

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
  }
  glm::vec3 getPosition() const { return m_position; }

  const glm::mat4 getProjectionMatrix() const { return m_projection; }

  void processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
      m_position += m_front * velocity;
    if (direction == BACKWARD)
      m_position -= m_front * velocity;
    if (direction == LEFT)
      m_position -= m_right * velocity;
    if (direction == RIGHT)
      m_position += m_right * velocity;
    if (direction == UP)
      m_position += m_worldUp * velocity;
    if (direction == DOWN)
      m_position -= m_worldUp * velocity;
  }

  void processMouseMovement(float xoffset, float yoffset,
                            bool constrainPitch = true) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
      if (m_pitch > 89.0f)
        m_pitch = 89.0f;
      if (m_pitch < -89.0f)
        m_pitch = -89.0f;
    }

    updateCameraVectors();
  }

  void setAspectRatio(float width, float height) {
    m_projection =
        glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.f);
  }

private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;

  float m_movementSpeed;
  float m_mouseSensitivity;

  glm::mat4 m_projection;

  void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
  }
};