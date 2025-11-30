#include "Graphics/Renderer.hpp"

#include <glad/glad.h>

#include "Core/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Shader.hpp"
#include "Scene/Model.hpp"

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);

  glGenBuffers(1, &m_CameraUBO);

  glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraDataUBOLayout), nullptr,
               GL_DYNAMIC_DRAW);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUBO);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::beginScene(const Camera &camera, Shader &shader) {
  shader.useShader();

  CameraDataUBOLayout cameraData;
  cameraData.view = camera.getViewMatrix();
  cameraData.projection = camera.getProjectionMatrix();
  cameraData.viewPos = camera.getPosition();

  glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraDataUBOLayout),
                  &cameraData);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::endScene() {}

void Renderer::submit(const std::shared_ptr<Model> &model, Transform &transform,
                      Shader &shader) {
  shader.setUniformMat4("model", transform.getModelMatrix());

  if (model) {
    model->draw(shader);
  }
}