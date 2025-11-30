#pragma once

#include <glm/glm.hpp>
#include <memory>

class Model;
class Shader;
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
  static void init();

  static void clear();
  static void setClearColor(const glm::vec4 &color);

  static void beginScene(const Camera &camera, Shader &shader);

  static void endScene();

  static void submit(const std::shared_ptr<Model> &model, Transform &transform,
                     Shader &shader);
};