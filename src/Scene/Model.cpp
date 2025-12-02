#include "Scene/Model.hpp"
#include "Core/Log.hpp"

#include <filesystem>

Model::Model(const std::string &path, ResourceManager &rm,
             std::shared_ptr<Shader> defaultShader)
    : m_resourceManager(rm), m_defaultShader(defaultShader) {
  loadModel(path);
}

void Model::addToScene(Scene &scene, const Transform &transform) {
  for (const auto &part : m_parts) {
    scene.addEntity(part.mesh, part.material, transform);
  }
}

void Model::loadModel(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    LOG_CORE_ERROR("Assimp Error ({0}): {1}", path, importer.GetErrorString());
    throw std::runtime_error("ERROR::MODEL::FAILED_TO_LOAD_SCENE");
  }

  std::filesystem::path p(path);
  m_directory = p.parent_path().string();
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_parts.push_back(processMesh(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Model::ModelPart Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex v;
    v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                  mesh->mVertices[i].z};

    if (mesh->HasNormals())
      v.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                  mesh->mNormals[i].z};
    else
      v.Normal = {0.0f, 0.0f, 0.0f};

    if (mesh->mTextureCoords[0])
      v.TexCoords = {mesh->mTextureCoords[0][i].x,
                     mesh->mTextureCoords[0][i].y};
    else
      v.TexCoords = {0.0f, 0.0f};

    vertices.push_back(v);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  auto myMesh = std::make_shared<Mesh>(vertices, indices);
  auto myMaterial = std::make_shared<Material>(m_defaultShader);

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    loadMaterialTextures(myMaterial, material, aiTextureType_DIFFUSE,
                         "texture_diffuse");
    loadMaterialTextures(myMaterial, material, aiTextureType_SPECULAR,
                         "texture_specular");
  }

  return {myMesh, myMaterial};
}

void Model::loadMaterialTextures(std::shared_ptr<Material> mat,
                                 aiMaterial *aiMat, aiTextureType type,
                                 const std::string &typeName) {
  // For simplicity, we only load the first texture of each type for now
  if (aiMat->GetTextureCount(type) > 0) {
    aiString str;
    aiMat->GetTexture(type, 0, &str);

    std::string filename = std::string(str.C_Str());
    std::string fullPath = m_directory + "/" + filename;

    auto texture = m_resourceManager.loadTexture(fullPath);
    mat->setTexture(typeName, texture);
  }
}