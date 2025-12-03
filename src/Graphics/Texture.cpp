#include "Graphics/Texture.hpp"
#include "Core/Log.hpp"

#include "stb_image.h"
#include <algorithm>
#include <cmath>
#include <glad/glad.h>

Texture::Texture(const std::string &textureFilePath) {
  LOG_CORE_TRACE("Loading texture: {0}", textureFilePath);

  glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);

  stbi_set_flip_vertically_on_load(true);
  unsigned char *texData =
      stbi_load(textureFilePath.c_str(), &m_width, &m_height, &m_BPP, 0);
  m_path = textureFilePath;

  bool loadedSuccessfully = false;

  if (texData) {
    GLenum internalFormat = 0;
    GLenum dataFormat = 0;

    if (m_BPP == 4) {
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGBA;
    } else if (m_BPP == 3) {
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGB;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    } else if (m_BPP == 1) {
      internalFormat = GL_R8;
      dataFormat = GL_RED;
    }

    if (internalFormat != 0) {
      glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER,
                          GL_LINEAR_MIPMAP_LINEAR);
      glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      GLsizei levels = 1 + static_cast<GLsizei>(std::floor(
                               std::log2(std::max(m_width, m_height))));

      glTextureStorage2D(m_textureID, levels, internalFormat, m_width,
                         m_height);

      glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, dataFormat,
                          GL_UNSIGNED_BYTE, texData);

      glGenerateTextureMipmap(m_textureID);

      if (m_BPP == 1) {
        glTextureParameteri(m_textureID, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTextureParameteri(m_textureID, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTextureParameteri(m_textureID, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTextureParameteri(m_textureID, GL_TEXTURE_SWIZZLE_A, GL_ONE);
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

    glTextureStorage2D(m_textureID, 1, GL_RGBA8, 1, 1);
    glTextureSubImage2D(m_textureID, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                        magenta);

    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }
}

Texture::~Texture() { glDeleteTextures(1, &m_textureID); }

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

void Texture::bind(unsigned int slot) { glBindTextureUnit(slot, m_textureID); }