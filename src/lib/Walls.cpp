#include "../include/Walls.hpp"
#include "../include/stb_image.h"
#include <GL/glew.h>
#include <iostream>

Walls::Walls() {
  vector<Vertex> vertices(8);

  float wallMinX = -10.0f;
  float wallMaxX = 10.0f;
  float wallMinZ = -15.0f;
  float wallMaxZ = 15.0f;
  float wallMinY = 0.0f;
  float wallMaxY = 7.5f;
  float wallUvRepeat = 0.5f;

  // Wierzchołki od lewego dolnego do prawgo dolnego
  // Zgodnie z ruchem wskazówek zegara

  // Dolne wierzchołki
  vertices[0].Pos = vec3(wallMinX, wallMinY, wallMinZ);
  vertices[0].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[0].TexCoords = vec2(wallUvRepeat, 0.0f);

  vertices[1].Pos = vec3(wallMinX, wallMinY, wallMaxZ);
  vertices[1].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[1].TexCoords = vec2(wallUvRepeat, wallUvRepeat);

  vertices[2].Pos = vec3(wallMaxX, wallMinY, wallMaxZ);
  vertices[2].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[2].TexCoords = vec2(wallUvRepeat, 0.0f);

  vertices[3].Pos = vec3(wallMaxX, wallMinY, wallMinZ);
  vertices[3].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[3].TexCoords = vec2(wallUvRepeat, wallUvRepeat);

  // Górne wierzchołki
  vertices[4].Pos = vec3(wallMinX, wallMaxY, wallMinZ);
  vertices[4].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[4].TexCoords = vec2(0.0f, 0.0f);

  vertices[5].Pos = vec3(wallMinX, wallMaxY, wallMaxZ);
  vertices[5].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[5].TexCoords = vec2(0.0f, wallUvRepeat);

  vertices[6].Pos = vec3(wallMaxX, wallMaxY, wallMaxZ);
  vertices[6].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[6].TexCoords = vec2(0.0f, 0.0f);

  vertices[7].Pos = vec3(wallMaxX, wallMaxY, wallMinZ);
  vertices[7].Normal = vec3(0.0f, 1.0f, 0.0f);
  vertices[7].TexCoords = vec2(0.0f, wallUvRepeat);

  vector<uint> indices = {
      0, 1, 4, 4, 5, 1, // Prawa ściana
      1, 2, 5, 5, 6, 2, // Frontowa ścian
      2, 3, 6, 6, 7, 3, // Lewa ściana
      3, 0, 7, 7, 4, 0  // Tylna ściana
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
