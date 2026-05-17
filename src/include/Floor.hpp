#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class Floor {
public:
  Floor();
  ~Floor();
  void Draw(Shader &shader);

private:
  Mesh *mesh;
};
