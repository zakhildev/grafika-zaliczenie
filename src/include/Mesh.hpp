#pragma once
#include "Shader.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include <string>
#include <sys/types.h>
#include <vector>

using namespace std;
using namespace glm;

typedef struct Vertex {
  vec3 Pos;
  vec3 Normal;
  vec2 TexCoords;
} Vertex;

typedef struct Texture {
  uint id;
  string type;
  string path;
} Texture;

class Mesh {
public:
  uint VAO;
  vector<Vertex> vertices;
  vector<uint> indices;
  vector<Texture> textures;

  Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
  void Draw(Shader &shader) const;

private:
  uint VBO, EBO;
  void setupMesh();
};
