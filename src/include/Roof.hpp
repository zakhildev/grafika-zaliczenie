#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class Roof {
public:
  Roof();
  ~Roof();
  void Draw(Shader &shader);

private:
  Mesh *mesh;
};
