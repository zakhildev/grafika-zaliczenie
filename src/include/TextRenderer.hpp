#pragma once

#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.hpp"

struct Character {
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2 Size;        // Size of glyph
  glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Offset to advance to next glyph
};

class TextRenderer {
public:
  std::map<char, Character> Characters;
  unsigned int VAO, VBO;

  TextRenderer(unsigned int width, unsigned int height);
  ~TextRenderer();

  void Load(std::string font, unsigned int fontSize);
  void RenderText(Shader &shader, std::string text, float x, float y,
                  float scale, glm::vec3 color);
};
