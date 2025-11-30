#pragma once

#include <glm/glm.hpp>
#include <memory>

class Model;
class Shader;
class Camera;
class Transform;

class Renderer {
public:
  static void init();

  static void clear();
  static void setClearColor(const glm::vec4 &color);

  static void beginScene(const Camera &camera, const glm::mat4 &projection,
                         Shader &shader);

  static void endScene();

  static void submit(const std::shared_ptr<Model> &model, Transform &transform,
                     Shader &shader);
};