#include "Graphics/Texture.hpp"
#include "Core/Log.hpp"

#include "stb_image.h"
#include <glad/glad.h>
#include <stdexcept>

Texture::Texture(const std::string &textureFilePath) {
  LOG_CORE_TRACE("Loading texture: {0}", textureFilePath);

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

  bool loadedSuccessfully = false;

  if (texData) {
    GLenum format = 0;
    if (m_BPP == 1)
      format = GL_RED;
    else if (m_BPP == 3) {
      format = GL_RGB;
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    } else if (m_BPP == 4)
      format = GL_RGBA;

    if (format != 0) {
      glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format,
                   GL_UNSIGNED_BYTE, texData);
      glGenerateMipmap(GL_TEXTURE_2D);

      if (m_BPP == 1) {
        int swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
      } else {
        int swizzleMask[] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
      }

      if (m_BPP == 3)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      LOG_CORE_INFO("Texture loaded: {0} ({1}x{2}, {3} channel(s))",
                    textureFilePath, m_width, m_height, m_BPP);
      loadedSuccessfully = true;
    } else {
      LOG_CORE_ERROR("Texture {0} has unsupported BPP: {1}", textureFilePath,
                     m_BPP);
    }

    stbi_image_free(texData);
  }

  if (!loadedSuccessfully) {
    LOG_CORE_ERROR("Failed to load texture {0}. Using fallback.",
                   textureFilePath);

    unsigned char magenta[] = {255, 0, 255, 255};

    m_width = 1;
    m_height = 1;
    m_BPP = 4;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 magenta);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
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