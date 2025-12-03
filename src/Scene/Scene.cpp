#include "Scene/Scene.hpp"

#include "Config.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"

Scene::Scene(const CameraConfig &cameraConfig, const RenderConfig &renderConfig)
    : m_camera(cameraConfig), m_lightPos(renderConfig.LightPosition) {}

void Scene::addEntity(const std::shared_ptr<Mesh> &mesh,
                      const std::shared_ptr<Material> &material,
                      const Transform &transform) {
  Entity e;
  e.mesh = mesh;
  e.material = material;
  e.transform = transform;
  m_entities.push_back(e);
}

void Scene::onUpdate(float dt, const InputManager &input) {
  if (input.isActionPressed(Action::MoveForward))
    m_camera.processKeyboard(CameraMovement::FORWARD, dt);
  if (input.isActionPressed(Action::MoveBackward))
    m_camera.processKeyboard(CameraMovement::BACKWARD, dt);
  if (input.isActionPressed(Action::MoveLeft))
    m_camera.processKeyboard(CameraMovement::LEFT, dt);
  if (input.isActionPressed(Action::MoveRight))
    m_camera.processKeyboard(CameraMovement::RIGHT, dt);
}

void Scene::onMouseView(float xoffset, float yoffset) {
  m_camera.processMouseMovement(xoffset, yoffset);
}