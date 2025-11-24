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

private:
  unsigned int m_textureID;
  int m_width, m_height, m_BPP;
};