#include "Scene/Scene.hpp"

#include "Config.hpp"
#include "Core/Input.hpp"
#include "Core/KeyCodes.hpp"

Scene::Scene() { m_camera = Camera(Config::Camera::StartPosition); }

void Scene::addEntity(const std::shared_ptr<Mesh> &mesh,
                      const std::shared_ptr<Material> &material,
                      const Transform &transform) {
  Entity e;
  e.mesh = mesh;
  e.material = material;
  e.transform = transform;
  m_entities.push_back(e);
}

void Scene::onUpdate(float dt) {
  if (Input::isKeyPressed(KeyCode::Up) || Input::isKeyPressed(KeyCode::W))
    m_camera.processKeyboard(CameraMovement::FORWARD, dt);
  if (Input::isKeyPressed(KeyCode::Down) || Input::isKeyPressed(KeyCode::S))
    m_camera.processKeyboard(CameraMovement::BACKWARD, dt);
  if (Input::isKeyPressed(KeyCode::Left) || Input::isKeyPressed(KeyCode::A))
    m_camera.processKeyboard(CameraMovement::LEFT, dt);
  if (Input::isKeyPressed(KeyCode::Right) || Input::isKeyPressed(KeyCode::D))
    m_camera.processKeyboard(CameraMovement::RIGHT, dt);
}

void Scene::onMouseView(float xoffset, float yoffset) {
  m_camera.processMouseMovement(xoffset, yoffset);
}