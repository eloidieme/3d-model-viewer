#include "Graphics/Renderer.hpp"
#include "Core/Log.hpp"
#include "Graphics/GeometryManager.hpp"
#include "Scene/Scene.hpp"

#include <algorithm>
#include <glad/glad.h>

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GeometryManager::get().init();

  LOG_CORE_INFO("Renderer initialized (Depth Test: ENABLED)");

  glCreateBuffers(1, &m_CameraUBO);
  glNamedBufferData(m_CameraUBO, sizeof(CameraDataUBOLayout), nullptr,
                    GL_DYNAMIC_DRAW);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUBO);
}

void Renderer::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::beginScene(Scene &scene) {
  m_activeScene = &scene;

  m_renderQueue.clear();

  Camera &camera = scene.getCamera();

  CameraDataUBOLayout cameraData;
  cameraData.view = camera.getViewMatrix();
  cameraData.projection = camera.getProjectionMatrix();
  cameraData.viewPos = camera.getPosition();

  glNamedBufferSubData(m_CameraUBO, 0, sizeof(CameraDataUBOLayout),
                       &cameraData);

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

  std::vector<RenderCommand> opaqueQueue;
  std::vector<RenderCommand> transparentQueue;

  opaqueQueue.reserve(m_renderQueue.size());
  transparentQueue.reserve(m_renderQueue.size());

  for (const auto &cmd : m_renderQueue) {
    if (cmd.material->isTransparent())
      transparentQueue.push_back(cmd);
    else
      opaqueQueue.push_back(cmd);
  }

  glBindVertexArray(GeometryManager::get().getGlobalVAO());

  auto drawCommands = [&](const std::vector<RenderCommand> &queue) {
    std::shared_ptr<Shader> currentShader = nullptr;

    for (const auto &cmd : queue) {
      if (!cmd.material || !cmd.mesh)
        continue;

      auto shader = cmd.material->getShader();

      if (shader != currentShader) {
        currentShader = shader;
        cmd.material->bind();

        if (m_activeScene) {
          currentShader->setUniformVec3("lightPos",
                                        m_activeScene->getLightPos());
          const auto &planes = m_activeScene->getClippingPlanes();
          currentShader->setUniformInt("u_ActiveClippingPlanes",
                                       static_cast<int>(planes.size()));

          for (size_t i = 0; i < planes.size(); i++) {
            std::string name = "u_ClippingPlanes[" + std::to_string(i) + "]";
            currentShader->setUniformVec4(name, planes[i]);
          }
        }
      } else {
        cmd.material->bind();
      }

      currentShader->setUniformMat4("model", cmd.transform);
      cmd.mesh->drawGeometry();
    }
  };

  glDepthMask(GL_TRUE);
  drawCommands(opaqueQueue);

  glDepthMask(GL_FALSE);
  drawCommands(transparentQueue);

  glDepthMask(GL_TRUE);

  glBindVertexArray(0);
}