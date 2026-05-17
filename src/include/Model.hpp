#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

using namespace std;

uint TextureFromFile(const char *path, const string &directory);

class Model {
public:
  Model(const char *path);
  void Draw(Shader &shader);

private:
  vector<Texture>
      texturesLoaded; // stores all the textures loaded so far, optimization to
                      // make sure textures aren't loaded more than once.
  vector<Mesh> meshes;
  string dir;

  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type,
                                       string typeName);
};
