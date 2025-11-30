#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

#include "Core/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Shader.hpp"
#include "Scene/Model.hpp"

void Renderer::init() { glEnable(GL_DEPTH_TEST); }

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::beginScene(const Camera &camera, const glm::mat4 &projection,
                          Shader &shader) {
  shader.useShader();

  shader.setUniformMat4("view", camera.getViewMatrix());
  shader.setUniformMat4("projection", projection);
  shader.setUniformVec3("viewPos", camera.getPosition());
}

void Renderer::endScene() {}

void Renderer::submit(const std::shared_ptr<Model> &model, Transform &transform,
                      Shader &shader) {
  shader.setUniformMat4("model", transform.getModelMatrix());

  if (model) {
    model->draw(shader);
  }
}