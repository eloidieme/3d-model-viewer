#pragma once

#include <string>

enum class TextureType { None = 0, Diffuse, Specular, Normal, Height };

class Texture {
public:
  Texture(const std::string &textureFilePath);
  ~Texture();

  Texture(const Texture &other) = delete;
  Texture &operator=(const Texture &other) = delete;

  Texture(Texture &&other) noexcept;
  Texture &operator=(Texture &&other) noexcept;

  void bind(unsigned int slot = 0);

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
  std::string getPath() const { return m_path; }
  TextureType getType() const { return m_type; }
  void setType(TextureType type) { m_type = type; }

private:
  unsigned int m_textureID;
  int m_width, m_height, m_BPP;
  std::string m_path;
  TextureType m_type = TextureType::None;
};