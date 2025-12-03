#include "Graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const CameraConfig &config)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_movementSpeed(config.MovementSpeed),
      m_mouseSensitivity(config.Sensitivity), m_fov(config.Fov),
      m_nearPlane(config.NearPlane), m_farPlane(config.FarPlane) {
  m_position = config.StartPosition;
  m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  m_yaw = -90.0f;
  m_pitch = 0.0f;
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::getPosition() const { return m_position; }

const glm::mat4 Camera::getProjectionMatrix() const { return m_projection; }

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
  float velocity = m_movementSpeed * deltaTime;
  if (direction == CameraMovement::FORWARD)
    m_position += m_front * velocity;
  if (direction == CameraMovement::BACKWARD)
    m_position -= m_front * velocity;
  if (direction == CameraMovement::LEFT)
    m_position -= m_right * velocity;
  if (direction == CameraMovement::RIGHT)
    m_position += m_right * velocity;
  if (direction == CameraMovement::UP)
    m_position += m_worldUp * velocity;
  if (direction == CameraMovement::DOWN)
    m_position -= m_worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset,
                                  bool constrainPitch) {
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

void Camera::setAspectRatio(float width, float height) {
  m_projection = glm::perspective(glm::radians(m_fov), width / height,
                                  m_nearPlane, m_farPlane);
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);

  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}