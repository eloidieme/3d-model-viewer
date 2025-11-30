#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Graphics/Mesh.hpp"
#include "Graphics/Texture.hpp"

class Shader;

class Model {
public:
  explicit Model(std::string modelFilePath);
  void draw(Shader &shader);

private:
  std::vector<Mesh> m_meshes;
  std::string m_directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<std::shared_ptr<Texture>>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                       TextureType typeName);
};