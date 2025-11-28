#pragma once

#include "Mesh.hpp"
#include "Texture.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

class Model {
public:
  explicit Model(std::string modelFilePath);
  void draw(Shader &shader);

private:
  std::vector<Mesh> m_meshes;
  std::string m_directory;

  std::vector<std::shared_ptr<Texture>> m_loadedTextures;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<std::shared_ptr<Texture>>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string name);
};