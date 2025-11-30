#include "Graphics/Texture.hpp"

#include <glad/glad.h>

#include "stb_image.h"

Texture::Texture(const std::string &textureFilePath) {
  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  unsigned char *texData =
      stbi_load(textureFilePath.c_str(), &m_width, &m_height, &m_BPP, 0);
  m_path = textureFilePath;

  GLenum format;
  if (m_BPP == 1) {
    format = GL_RED;
  } else if (m_BPP == 3) {
    format = GL_RGB;
  } else if (m_BPP == 4) {
    format = GL_RGBA;
  } else {
    throw std::runtime_error("ERROR::TEXTURE::UNKNOWN_FORMAT");
  }

  if (texData) {
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format,
                 GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE_DATA");
  }

  stbi_image_free(texData);
}
Texture::~Texture() { glDeleteTextures(1, &m_textureID); };

Texture::Texture(Texture &&other) noexcept {
  m_textureID = other.m_textureID;
  m_width = other.m_width;
  m_height = other.m_height;
  m_BPP = other.m_BPP;

  other.m_textureID = 0;
}
Texture &Texture::operator=(Texture &&other) noexcept {

  if (this != &other) {
    glDeleteTextures(1, &m_textureID);

    m_textureID = other.m_textureID;
    m_width = other.m_width;
    m_height = other.m_height;
    m_BPP = other.m_BPP;

    other.m_textureID = 0;
  }

  return *this;
}

void Texture::bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}