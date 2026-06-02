#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class Walls {
public:
  Walls();
  ~Walls();
  void Draw(Shader &shader);

private:
  Mesh *mesh;
};
