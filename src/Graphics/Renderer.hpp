#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Graphics/Shader.hpp"
#include "Scene/Model.hpp"

class Camera;
class Transform;

struct CameraDataUBOLayout {
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 viewPos;
  float padding;
};

class Renderer {
public:
  void init();

  void clear();
  void setClearColor(const glm::vec4 &color);

  void beginScene(const Camera &camera, Shader &shader);

  void endScene();

  void submit(const std::shared_ptr<Model> &model, Transform &transform,
              Shader &shader);

private:
  unsigned int m_CameraUBO = 0;
};