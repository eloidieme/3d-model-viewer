#pragma once

#include <string>

class Texture {
public:
  Texture(const std::string &textureFilePath);
  ~Texture();

  Texture(const Texture &other) = delete;
  Texture &operator=(const Texture &other) = delete;

  Texture(Texture &&other);
  Texture &operator=(Texture &&other);

  void bind(unsigned int slot = 0);

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
  std::string getPath() const { return m_path; }
  std::string getType() const { return m_type; }
  void setType(std::string type) { m_type = type; }

private:
  unsigned int m_textureID;
  int m_width, m_height, m_BPP;
  std::string m_path;
  std::string m_type;
};