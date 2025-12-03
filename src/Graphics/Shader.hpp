#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
public:
  Shader(const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath);
  ~Shader();

  void reload();

  void useShader();

  void setUniformFloat(const std::string &name, float value) const;
  void setUniformInt(const std::string &name, int value) const;
  void setUniformBool(const std::string &name, bool value) const;
  void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
  void setUniformVec4(const std::string &name, const glm::vec4 &value) const;
  void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  unsigned int m_programID = 0;

  std::string m_vertexPath;
  std::string m_fragmentPath;

  mutable std::unordered_map<std::string, int> m_uniformLocationCache;
  int getUniformLocation(const std::string &name) const;

  void compile();
};