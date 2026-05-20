#include "../include/Mesh.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <string>
#include <vector>

Mesh::Mesh(vector<Vertex> vertices, vector<uint> indices,
           vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Tu można dać vertices[0] bo struktura jest ciągła w pamięci
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Color));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Tangent));

  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Bitangent));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) const {
  uint diffuseNum = 1;
  uint specularNum = 1;

  for (uint i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    string number;
    string type = textures[i].type;
    if (type == "texture_diffuse")
      number = to_string(diffuseNum++);
    else if (type == "texture_specular")
      number = to_string(specularNum++);
    glUniform1i(glGetUniformLocation(shader.getID(), (type + number).c_str()),
                i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<uint>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}