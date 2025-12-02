#pragma once

#include "Graphics/ResourceManager.hpp"
#include "Scene/Scene.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model {
public:
  Model(const std::string &path, ResourceManager &rm,
        std::shared_ptr<Shader> defaultShader);
  void addToScene(Scene &scene, const Transform &transform = Transform());

private:
  struct ModelPart {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
  };

  std::vector<ModelPart> m_parts;
  std::string m_directory;

  ResourceManager &m_resourceManager;
  std::shared_ptr<Shader> m_defaultShader;

  void loadModel(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  ModelPart processMesh(aiMesh *mesh, const aiScene *scene);

  void loadMaterialTextures(std::shared_ptr<Material> mat, aiMaterial *aiMat,
                            aiTextureType type, const std::string &typeName);
};