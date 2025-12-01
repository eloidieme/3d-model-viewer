#include "Graphics/ResourceManager.hpp"

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string &path,
                                                      TextureType typeName) {
  if (m_textures.find(path) != m_textures.end()) {
    return m_textures[path];
  }

  auto texture = std::make_shared<Texture>(path);
  texture->setType(typeName);

  m_textures[path] = texture;
  return texture;
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &path) {
  if (m_textures.find(path) != m_textures.end())
    return m_textures[path];
  return nullptr;
}

std::shared_ptr<Shader>
ResourceManager::loadShader(const std::string &name,
                            const std::string &vShaderFile,
                            const std::string &fShaderFile) {
  if (m_shaders.find(name) != m_shaders.end()) {
    return m_shaders[name];
  }

  auto shader = std::make_shared<Shader>(vShaderFile, fShaderFile);
  m_shaders[name] = shader;
  return shader;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string &name) {
  if (m_shaders.find(name) != m_shaders.end())
    return m_shaders[name];
  return nullptr;
}

void ResourceManager::clear() {
  m_shaders.clear();
  m_textures.clear();
}