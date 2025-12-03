#pragma once

#include "Config.hpp"

#include <glm/glm.hpp>
#include <vector>

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {

public:
  Camera(const CameraConfig &config);

  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const;
  const glm::mat4 getProjectionMatrix() const;

  void processKeyboard(CameraMovement direction, float deltaTime);
  void processMouseMovement(float xoffset, float yoffset,
                            bool constrainPitch = true);

  void setAspectRatio(float width, float height);

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
  float m_fov;
  float m_nearPlane;
  float m_farPlane;

  glm::mat4 m_projection;

  void updateCameraVectors();
};