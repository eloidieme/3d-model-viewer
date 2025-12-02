#include "Graphics/Material.hpp"
#include "Core/Log.hpp"

Material::Material(std::shared_ptr<Shader> shader)
    : m_shader(std::move(shader)) {}

void Material::bind() {
  if (!m_shader)
    return;

  m_shader->useShader();

  for (const auto &[name, value] : m_uniformsFloat)
    m_shader->setUniformFloat(name, value);
  for (const auto &[name, value] : m_uniformsInt)
    m_shader->setUniformInt(name, value);
  for (const auto &[name, value] : m_uniformsBool)
    m_shader->setUniformBool(name, value);
  for (const auto &[name, value] : m_uniformsVec3)
    m_shader->setUniformVec3(name, value);
  for (const auto &[name, value] : m_uniformsVec4)
    m_shader->setUniformVec4(name, value);
  for (const auto &[name, value] : m_uniformsMat4)
    m_shader->setUniformMat4(name, value);

  unsigned int slot = 0;
  for (const auto &[name, texture] : m_textures) {
    if (texture) {
      texture->bind(slot);
      m_shader->setUniformInt(name, slot);
      slot++;
    }
  }
}

void Material::unbind() {
  // We keep it empty because we trust the Renderer to bind correctly.
  // By not explicitely unbinding, we save on driver overhead.
}

void Material::setFloat(const std::string &name, float value) {
  m_uniformsFloat[name] = value;
}
void Material::setInt(const std::string &name, int value) {
  m_uniformsInt[name] = value;
}
void Material::setBool(const std::string &name, bool value) {
  m_uniformsBool[name] = value;
}
void Material::setVec3(const std::string &name, const glm::vec3 &value) {
  m_uniformsVec3[name] = value;
}
void Material::setVec4(const std::string &name, const glm::vec4 &value) {
  m_uniformsVec4[name] = value;
}
void Material::setMat4(const std::string &name, const glm::mat4 &value) {
  m_uniformsMat4[name] = value;
}

void Material::setTexture(const std::string &name,
                          std::shared_ptr<Texture> texture) {
  m_textures[name] = texture;
}