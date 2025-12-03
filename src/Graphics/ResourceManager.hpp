#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class ResourceManager {
public:
  std::shared_ptr<Texture>
  loadTexture(const std::string &path,
              TextureType typeName = TextureType::Diffuse);
  std::shared_ptr<Texture> getTexture(const std::string &path);

  std::shared_ptr<Shader> loadShader(const std::string &name,
                                     const std::string &vShaderFile,
                                     const std::string &fShaderFile);
  std::shared_ptr<Shader> getShader(const std::string &name);

  void clear();

  void reloadAllShaders();

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};