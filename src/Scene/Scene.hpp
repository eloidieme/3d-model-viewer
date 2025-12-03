#pragma once

#include "Config.hpp"
#include "Core/InputManager.hpp"
#include "Core/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct Entity {
  Transform transform;
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Material> material;
};

class Scene {
public:
  Scene(const CameraConfig &cameraConfig, const RenderConfig &renderConfig);
  ~Scene() = default;

  void onUpdate(float dt, const InputManager &input);

  void addEntity(const std::shared_ptr<Mesh> &mesh,
                 const std::shared_ptr<Material> &material,
                 const Transform &transform = Transform());

  Camera &getCamera() { return m_camera; }
  const std::vector<Entity> &getEntities() const { return m_entities; }

  glm::vec3 &getLightPos() { return m_lightPos; }
  std::vector<glm::vec4> &getClippingPlanes() { return m_clippingPlanes; }

  void onMouseView(float xoffset, float yoffset);

private:
  Camera m_camera;
  std::vector<Entity> m_entities;

  glm::vec3 m_lightPos;
  std::vector<glm::vec4> m_clippingPlanes;
};