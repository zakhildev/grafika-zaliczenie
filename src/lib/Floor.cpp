#include "../include/Floor.hpp"
#include "../include/stb_image.h"
#include <GL/glew.h>
#include <iostream>

Floor::Floor() {
  vector<Vertex> vertices(4);

  float size = 15.0f;
  float uv_repeat = 5.0f;

  vertices[0].Pos = vec3(-size, 0.0f, size);
  vertices[0].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[0].TexCoords = vec2(0.0f, 0.0f);

  vertices[1].Pos = vec3(size, 0.0f, size);
  vertices[1].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[1].TexCoords = vec2(uv_repeat, 0.0f);

  vertices[2].Pos = vec3(size, 0.0f, -size);
  vertices[2].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[2].TexCoords = vec2(uv_repeat, uv_repeat);

  vertices[3].Pos = vec3(-size, 0.0f, -size);
  vertices[3].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[3].TexCoords = vec2(0.0f, uv_repeat);

  vector<uint> indices = {0, 1, 2, 0, 2, 3};

  vector<Texture> textures;

  int width, height, channels;

  // Diffuse texture
  Texture diffuse;
  diffuse.type = "texture_diffuse";
  diffuse.path = "textures/floor_diffuse.jpg";
  unsigned char *tex_diffuse =
      stbi_load(diffuse.path.c_str(), &width, &height, &channels, 0);
  if (tex_diffuse) {
    glGenTextures(1, &diffuse.id);
    glBindTexture(GL_TEXTURE_2D, diffuse.id);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, tex_diffuse);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(tex_diffuse);
    textures.push_back(diffuse);
  } else {
    std::cerr << "Failed to load floor diffuse texture." << std::endl;
  }

  Texture specular;
  specular.type = "texture_specular";
  specular.path = "textures/floor_specular.jpg";
  unsigned char *tex_specular =
      stbi_load(specular.path.c_str(), &width, &height, &channels, 0);
  if (tex_specular) {
    glGenTextures(1, &specular.id);
    glBindTexture(GL_TEXTURE_2D, specular.id);
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, tex_specular);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(tex_specular);
    textures.push_back(specular);
  } else {
    std::cerr << "Failed to load floor specular texture." << std::endl;
  }

  mesh = new Mesh(vertices, indices, textures);
}

Floor::~Floor() { delete mesh; }

void Floor::Draw(Shader &shader) { mesh->Draw(shader); }
