#include "Graphics/ResourceManager.hpp"

#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Shader>>
    ResourceManager::s_shaders;
std::unordered_map<std::string, std::shared_ptr<Texture>>
    ResourceManager::s_textures;

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string &path,
                                                      TextureType typeName) {
  if (s_textures.find(path) != s_textures.end()) {
    return s_textures[path];
  }

  auto texture = std::make_shared<Texture>(path);
  texture->setType(typeName);

  s_textures[path] = texture;
  return texture;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &path) {
  if (s_textures.find(path) != s_textures.end())
    return s_textures[path];
  return nullptr;
}

std::shared_ptr<Shader>
ResourceManager::loadShader(const std::string &name,
                            const std::string &vShaderFile,
                            const std::string &fShaderFile) {
  if (s_shaders.find(name) != s_shaders.end()) {
    return s_shaders[name];
  }

  auto shader = std::make_shared<Shader>(vShaderFile, fShaderFile);
  s_shaders[name] = shader;
  return shader;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string &name) {
  if (s_shaders.find(name) != s_shaders.end())
    return s_shaders[name];
  return nullptr;
}

void ResourceManager::clear() {
  s_shaders.clear();
  s_textures.clear();
}