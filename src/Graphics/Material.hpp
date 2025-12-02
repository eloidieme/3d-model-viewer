#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class Material {
public:
  Material(std::shared_ptr<Shader> shader);
  ~Material() = default;

  void bind();
  void unbind();

  void setFloat(const std::string &name, float value);
  void setInt(const std::string &name, int value);
  void setBool(const std::string &name, bool value);
  void setVec3(const std::string &name, const glm::vec3 &value);
  void setVec4(const std::string &name, const glm::vec4 &value);
  void setMat4(const std::string &name, const glm::mat4 &value);

  void setTexture(const std::string &name, std::shared_ptr<Texture> texture);

  std::shared_ptr<Shader> getShader() const { return m_shader; }

private:
  std::shared_ptr<Shader> m_shader;

  std::unordered_map<std::string, float> m_uniformsFloat;
  std::unordered_map<std::string, int> m_uniformsInt;
  std::unordered_map<std::string, bool> m_uniformsBool;
  std::unordered_map<std::string, glm::vec3> m_uniformsVec3;
  std::unordered_map<std::string, glm::vec4> m_uniformsVec4;
  std::unordered_map<std::string, glm::mat4> m_uniformsMat4;

  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};