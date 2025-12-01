#include "Graphics/Shader.hpp"
#include "Core/Log.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath) {
  int success;
  char infoLog[512];

  std::ifstream vShaderFile, fShaderFile;
  vShaderFile.open(vertexShaderPath);
  fShaderFile.open(fragmentShaderPath);
  if (!vShaderFile.is_open()) {
    throw std::runtime_error(
        "ERROR::FILE::FAILED_TO_OPEN_VERTEX_SHADER_SOURCE");
  }
  if (!fShaderFile.is_open()) {
    throw std::runtime_error(
        "ERROR::FILE::FAILED_TO_OPEN_FRAGMENT_SHADER_SOURCE");
  }

  std::stringstream vShaderStream, fShaderStream;
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();

  std::string vShaderSourceStr = vShaderStream.str();
  std::string fShaderSourceStr = fShaderStream.str();

  const char *vShaderSource = vShaderSourceStr.c_str();
  const char *fShaderSource = fShaderSourceStr.c_str();

  unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, nullptr);
  glCompileShader(vShader);
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
    LOG_CORE_ERROR("Vertex Shader Compile Error ({0}):\n{1}", vertexShaderPath,
                   infoLog);
    throw std::runtime_error("ERROR::SHADER::FAILED_TO_COMPILE_VERTEX_SHADER");
  }

  unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, nullptr);
  glCompileShader(fShader);
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
    LOG_CORE_ERROR("Fragment Shader Compile Error ({0}):\n{1}",
                   fragmentShaderPath, infoLog);
    throw std::runtime_error(
        "ERROR::SHADER::FAILED_TO_COMPILE_FRAGMENT_SHADER");
  }

  m_programID = glCreateProgram();
  glAttachShader(m_programID, vShader);
  glAttachShader(m_programID, fShader);
  glLinkProgram(m_programID);
  glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
    LOG_CORE_ERROR("Shader Link Error:\n{0}", infoLog);
    throw std::runtime_error("ERROR::SHADER::FAILED_TO_LINK_PROGRAM");
  }

  unsigned int uniformBlockIndex =
      glGetUniformBlockIndex(m_programID, "CameraData");

  if (uniformBlockIndex != GL_INVALID_INDEX) {
    glUniformBlockBinding(m_programID, uniformBlockIndex, 0);
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  vShaderFile.close();
  fShaderFile.close();
}
Shader::~Shader() { glDeleteProgram(m_programID); }

void Shader::useShader() { glUseProgram(m_programID); }

int Shader::getUniformLocation(const std::string &name) const {
  if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
    return m_uniformLocationCache[name];
  }

  int location = glGetUniformLocation(m_programID, name.c_str());
  if (location == -1) {
    LOG_CORE_WARN("Uniform '{0}' not found or unused in shader!", name);
  }

  m_uniformLocationCache[name] = location;
  return location;
}

void Shader::setUniformFloat(const std::string &name, float value) const {
  glUniform1f(getUniformLocation(name), value);
}
void Shader::setUniformInt(const std::string &name, int value) const {
  glUniform1i(getUniformLocation(name), value);
}
void Shader::setUniformBool(const std::string &name, bool value) const {
  glUniform1i(getUniformLocation(name), (int)value);
}
void Shader::setUniformVec3(const std::string &name,
                            const glm::vec3 &value) const {
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}
void Shader::setUniformVec4(const std::string &name,
                            const glm::vec4 &value) const {
  glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}
void Shader::setUniformMat4(const std::string &name,
                            const glm::mat4 &mat) const {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(mat));
}
