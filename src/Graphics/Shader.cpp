#include "Graphics/Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath) {
  // For error handling
  int success;
  char infoLog[512];

  // open the files using the path
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

  // init streams and perform a buffered read from the files
  std::stringstream vShaderStream, fShaderStream;
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();

  // convert streams into strings
  std::string vShaderSourceStr = vShaderStream.str();
  std::string fShaderSourceStr = fShaderStream.str();

  const char *vShaderSource = vShaderSourceStr.c_str();
  const char *fShaderSource = fShaderSourceStr.c_str();

  // create and compile shaders
  unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, nullptr);
  glCompileShader(vShader);
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
    std::cerr << infoLog << std::endl;
    throw std::runtime_error("ERROR::SHADER::FAILED_TO_COMPILE_VERTEX_SHADER");
  }

  unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, nullptr);
  glCompileShader(fShader);
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
    std::cerr << infoLog << std::endl;
    throw std::runtime_error(
        "ERROR::SHADER::FAILED_TO_COMPILE_FRAGMENT_SHADER");
  }

  // create program and link
  m_programID = glCreateProgram();
  glAttachShader(m_programID, vShader);
  glAttachShader(m_programID, fShader);
  glLinkProgram(m_programID);
  glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
    std::cerr << infoLog << std::endl;
    throw std::runtime_error("ERROR::SHADER::FAILED_TO_LINK_PROGRAM");
  }

  unsigned int uniformBlockIndex =
      glGetUniformBlockIndex(m_programID, "CameraData");

  if (uniformBlockIndex != GL_INVALID_INDEX) {
    glUniformBlockBinding(m_programID, uniformBlockIndex, 0);
  }

  // delete shaders and close files
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
    std::cerr << "WARNING: Uniform '" << name << "' doesn't exist!"
              << std::endl;
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
