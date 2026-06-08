#include "../include/Walls.hpp"
#include "../include/stb_image.h"
#include <GL/glew.h>
#include <iostream>

Walls::Walls() {
  vector<Vertex> vertices(16);

  float wallMinX = -10.0f;
  float wallMaxX = 10.0f;
  float wallMinZ = -15.0f;
  float wallMaxZ = 15.0f;
  float wallMinY = 0.0f;
  float wallMaxY = 7.5f;
  float wallUvRepeat = 0.5f;

  // Przygotowujemy 16 wierzchołków (po 4 dla każdej z 4 ścian)
  vertices.resize(16);

  vec3 normLeft = vec3(1.0f, 0.0f, 0.0f);
  vertices[0].Pos = vec3(wallMinX, wallMinY, wallMinZ);
  vertices[1].Pos = vec3(wallMinX, wallMinY, wallMaxZ);
  vertices[2].Pos = vec3(wallMinX, wallMaxY, wallMaxZ);
  vertices[3].Pos = vec3(wallMinX, wallMaxY, wallMinZ);

  vertices[0].TexCoords = vec2(0.0f, 0.0f);
  vertices[1].TexCoords = vec2(wallUvRepeat, 0.0f);
  vertices[2].TexCoords = vec2(wallUvRepeat, wallUvRepeat);
  vertices[3].TexCoords = vec2(0.0f, wallUvRepeat);

  for (int i = 0; i < 4; i++)
    vertices[i].Normal = normLeft;

  vec3 normFront = vec3(0.0f, 0.0f, -1.0f);
  vertices[4].Pos = vec3(wallMinX, wallMinY, wallMaxZ);
  vertices[5].Pos = vec3(wallMaxX, wallMinY, wallMaxZ);
  vertices[6].Pos = vec3(wallMaxX, wallMaxY, wallMaxZ);
  vertices[7].Pos = vec3(wallMinX, wallMaxY, wallMaxZ);

  vertices[4].TexCoords = vec2(0.0f, 0.0f);
  vertices[5].TexCoords = vec2(wallUvRepeat, 0.0f);
  vertices[6].TexCoords = vec2(wallUvRepeat, wallUvRepeat);
  vertices[7].TexCoords = vec2(0.0f, wallUvRepeat);

  for (int i = 4; i < 8; i++)
    vertices[i].Normal = normFront;

  vec3 normRight = vec3(-1.0f, 0.0f, 0.0f);
  vertices[8].Pos = vec3(wallMaxX, wallMinY, wallMaxZ);
  vertices[9].Pos = vec3(wallMaxX, wallMinY, wallMinZ);
  vertices[10].Pos = vec3(wallMaxX, wallMaxY, wallMinZ);
  vertices[11].Pos = vec3(wallMaxX, wallMaxY, wallMaxZ);

  vertices[8].TexCoords = vec2(0.0f, 0.0f);
  vertices[9].TexCoords = vec2(wallUvRepeat, 0.0f);
  vertices[10].TexCoords = vec2(wallUvRepeat, wallUvRepeat);
  vertices[11].TexCoords = vec2(0.0f, wallUvRepeat);

  for (int i = 8; i < 12; i++)
    vertices[i].Normal = normRight;

  vec3 normBack = vec3(0.0f, 0.0f, 1.0f);
  vertices[12].Pos = vec3(wallMaxX, wallMinY, wallMinZ);
  vertices[13].Pos = vec3(wallMinX, wallMinY, wallMinZ);
  vertices[14].Pos = vec3(wallMinX, wallMaxY, wallMinZ);
  vertices[15].Pos = vec3(wallMaxX, wallMaxY, wallMinZ);

  vertices[12].TexCoords = vec2(0.0f, 0.0f);
  vertices[13].TexCoords = vec2(wallUvRepeat, 0.0f);
  vertices[14].TexCoords = vec2(wallUvRepeat, wallUvRepeat);
  vertices[15].TexCoords = vec2(0.0f, wallUvRepeat);

  for (int i = 12; i < 16; i++)
    vertices[i].Normal = normBack;

  vector<uint> indices = {
      0,  1,  2,  2,  3,  0, // Lewa (-X)
      4,  5,  6,  6,  7,  4, // Front (+Z)
      8,  9,  10, 10, 11, 8, // Prawa (+X)
      12, 13, 14, 14, 15, 12 // Tył (-Z)
  };

  vector<Texture> textures;

  int w, h, c;
  Texture diffuse;
  diffuse.type = "texture_diffuse";
  diffuse.path = "textures/wall_diffuse.jpg";
  unsigned char *tex_diffuse = stbi_load(diffuse.path.c_str(), &w, &h, &c, 0);
  if (tex_diffuse) {
    glGenTextures(1, &diffuse.id);
    glBindTexture(GL_TEXTURE_2D, diffuse.id);
    GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
                 tex_diffuse);
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
  specular.path = "textures/wall_specular.jpg";
  unsigned char *tex_specular = stbi_load(specular.path.c_str(), &w, &h, &c, 0);
  if (tex_specular) {
    glGenTextures(1, &specular.id);
    glBindTexture(GL_TEXTURE_2D, specular.id);
    GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
                 tex_specular);
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

Walls::~Walls() { delete mesh; };

void Walls::Draw(Shader &shader) { mesh->Draw(shader); }
