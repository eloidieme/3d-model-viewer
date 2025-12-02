#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Scene/Scene.hpp"

struct RenderCommand {
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Material> material;
  glm::mat4 transform;

  float distanceToCamera;
};

struct CameraDataUBOLayout {
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 viewPos;
  float padding;
};

class Renderer {
public:
  void init();

  void setClearColor(const glm::vec4 &color);
  void clear();

  void beginScene(Scene &scene);
  void endScene();

private:
  unsigned int m_CameraUBO = 0;

  std::vector<RenderCommand> m_renderQueue;

  void submit(const std::shared_ptr<Mesh> &mesh,
              const std::shared_ptr<Material> &material,
              const glm::mat4 &transform);
};