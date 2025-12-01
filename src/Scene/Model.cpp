#include "Scene/Model.hpp"
#include "Core/Log.hpp"
#include "Graphics/ResourceManager.hpp"

#include <filesystem>
#include <glm/glm.hpp>

Model::Model(std::string modelFilePath, ResourceManager &resourceManager)
    : m_resourceManager(resourceManager) {
  loadModel(modelFilePath);
}

void Model::draw(Shader &shader) {
  for (unsigned int i = 0; i < m_meshes.size(); i++) {
    m_meshes[i].draw(shader);
  }
}

void Model::loadModel(std::string path) {
  LOG_CORE_TRACE("Loading model from: {0}", path);

  Assimp::Importer importer;

  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    LOG_CORE_ERROR("Assimp Error ({0}): {1}", path, importer.GetErrorString());
    throw std::runtime_error("ERROR::MODEL::FAILED_TO_LOAD_SCENE");
  }

  std::filesystem::path p(path);
  m_directory = p.parent_path().string();

  processNode(scene->mRootNode, scene);

  LOG_CORE_INFO("Model loaded successfully. Meshes: {0}", m_meshes.size());
}
void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  const bool hasNormal = mesh->HasNormals();
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    glm::vec3 position{mesh->mVertices[i].x, mesh->mVertices[i].y,
                       mesh->mVertices[i].z};

    glm::vec3 normal{0.0, 0.0, 0.0};
    if (hasNormal) {
      normal.x = mesh->mNormals[i].x;
      normal.y = mesh->mNormals[i].y;
      normal.z = mesh->mNormals[i].z;
    }

    glm::vec2 texCoords{0.0, 0.0};
    if (mesh->mTextureCoords[0] != nullptr) {
      texCoords.x = mesh->mTextureCoords[0][i].x;
      texCoords.y = mesh->mTextureCoords[0][i].y;
    }

    struct Vertex v;
    v.Position = position;
    v.Normal = normal;
    v.TexCoords = texCoords;

    vertices.push_back(v);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  auto diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                              TextureType::Diffuse);
  auto specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                               TextureType::Specular);

  textures.reserve(diffuseTextures.size() + specularTextures.size());
  textures.insert(textures.end(), diffuseTextures.begin(),
                  diffuseTextures.end());
  textures.insert(textures.end(), specularTextures.begin(),
                  specularTextures.end());

  return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                            TextureType typeName) {
  std::vector<std::shared_ptr<Texture>> ret;

  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    std::filesystem::path directory(m_directory);
    std::filesystem::path fileName(str.C_Str());
    std::string fullPath = (directory / fileName).string();

    std::shared_ptr<Texture> texture =
        m_resourceManager.loadTexture(fullPath, typeName);
    ret.push_back(texture);
  }

  return ret;
}