#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class ResourceManager {
public:
  static std::shared_ptr<Texture>
  loadTexture(const std::string &path,
              TextureType typeName = TextureType::Diffuse);
  static std::shared_ptr<Texture> getTexture(const std::string &path);

  static std::shared_ptr<Shader> loadShader(const std::string &name,
                                            const std::string &vShaderFile,
                                            const std::string &fShaderFile);
  static std::shared_ptr<Shader> getShader(const std::string &name);

  static void clear();

private:
  ResourceManager() {}

  static std::unordered_map<std::string, std::shared_ptr<Shader>> s_shaders;
  static std::unordered_map<std::string, std::shared_ptr<Texture>> s_textures;
};