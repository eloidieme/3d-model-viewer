#include "Graphics/Renderer.hpp"
#include "Core/Log.hpp"
#include "Scene/Scene.hpp"

#include <algorithm>
#include <glad/glad.h>

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  LOG_CORE_INFO("Renderer initialized (Depth Test: ENABLED)");

  glGenBuffers(1, &m_CameraUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraDataUBOLayout), nullptr,
               GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::beginScene(Scene &scene) {
  m_renderQueue.clear();

  Camera &camera = scene.getCamera();

  CameraDataUBOLayout cameraData;
  cameraData.view = camera.getViewMatrix();
  cameraData.projection = camera.getProjectionMatrix();
  cameraData.viewPos = camera.getPosition();

  glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraDataUBOLayout),
                  &cameraData);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  for (const auto &entity : scene.getEntities()) {
    if (entity.mesh && entity.material) {
      submit(entity.mesh, entity.material, entity.transform.getModelMatrix());
    }
  }
}

void Renderer::submit(const std::shared_ptr<Mesh> &mesh,
                      const std::shared_ptr<Material> &material,
                      const glm::mat4 &transform) {
  RenderCommand cmd;
  cmd.mesh = mesh;
  cmd.material = material;
  cmd.transform = transform;
  m_renderQueue.push_back(cmd);
}

void Renderer::endScene() {
  std::sort(m_renderQueue.begin(), m_renderQueue.end(),
            [](const RenderCommand &a, const RenderCommand &b) {
              return a.material->getShader() < b.material->getShader();
            });

  std::shared_ptr<Shader> lastShader = nullptr;

  for (const auto &cmd : m_renderQueue) {
    if (!cmd.material || !cmd.mesh)
      continue;

    std::shared_ptr<Shader> currentShader = cmd.material->getShader();

    cmd.material->bind();
    currentShader->setUniformMat4("model", cmd.transform);
    cmd.mesh->drawGeometry();
  }
}
