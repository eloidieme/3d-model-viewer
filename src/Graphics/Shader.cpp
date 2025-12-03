#include "Graphics/Shader.hpp"
#include "Core/Log.hpp"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

Shader::Shader(const std::string &vertexShaderPath,
               const std::string &fragmentShaderPath)
    : m_vertexPath(vertexShaderPath), m_fragmentPath(fragmentShaderPath) {

  compile();
}

Shader::~Shader() {
  if (m_programID != 0)
    glDeleteProgram(m_programID);
}

void Shader::reload() {
  LOG_CORE_WARN("Reloading Shader: {0}", m_vertexPath);

  if (m_programID != 0)
    glDeleteProgram(m_programID);
  m_uniformLocationCache.clear();

  compile();
}

void Shader::compile() {
  std::string vShaderSourceStr, fShaderSourceStr;
  std::ifstream vShaderFile, fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(m_vertexPath);
    fShaderFile.open(m_fragmentPath);

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderSourceStr = vShaderStream.str();
    fShaderSourceStr = fShaderStream.str();

    vShaderFile.close();
    fShaderFile.close();
  } catch (std::ifstream::failure &e) {
    LOG_CORE_ERROR("SHADER FILE ERROR: Not successfully read. Path: {0}",
                   m_vertexPath);
    return;
  }

  const char *vShaderSource = vShaderSourceStr.c_str();
  const char *fShaderSource = fShaderSourceStr.c_str();

  unsigned int vShader, fShader;
  int success;
  char infoLog[512];

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, nullptr);
  glCompileShader(vShader);
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
    LOG_CORE_ERROR("VERTEX COMPILATION FAILED ({0}):\n{1}", m_vertexPath,
                   infoLog);
  }

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, nullptr);
  glCompileShader(fShader);
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
    LOG_CORE_ERROR("FRAGMENT COMPILATION FAILED ({0}):\n{1}", m_fragmentPath,
                   infoLog);
  }

  m_programID = glCreateProgram();
  glAttachShader(m_programID, vShader);
  glAttachShader(m_programID, fShader);
  glLinkProgram(m_programID);

  glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
    LOG_CORE_ERROR("SHADER LINKING FAILED:\n{0}", infoLog);
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  unsigned int uniformBlockIndex =
      glGetUniformBlockIndex(m_programID, "CameraData");
  if (uniformBlockIndex != GL_INVALID_INDEX) {
    glUniformBlockBinding(m_programID, uniformBlockIndex, 0);
  }
}

void Shader::useShader() { glUseProgram(m_programID); }

int Shader::getUniformLocation(const std::string &name) const {
  if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
    return m_uniformLocationCache[name];
  }
  int location = glGetUniformLocation(m_programID, name.c_str());
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