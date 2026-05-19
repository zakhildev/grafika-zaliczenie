#include "../include/Shader.hpp"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. Retrieve the vertex/fragment source code from filePath
  string vertexCode;
  string fragmentCode;
  ifstream vShaderFile;
  ifstream fShaderFile;

  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
  fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

  try {
    // Open shader files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    stringstream vShaderStream, fShaderStream;

    // Read files into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert streams into strings
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (ifstream::failure error) {
    cerr << "[Shader] Error, unable to read shader files: " << error.what()
         << endl;
  }

  // Convert shader code to C strings (const char *)
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  uint vertex, fragment;
  int ok;
  char infoLog[512];

  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
    cerr << "[Shader] Error, vertex shader compilation failed: " << infoLog
         << endl;
  }

  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
    cerr << "[Shader] Error, fragment shader compilation failed: " << infoLog
         << endl;
  }

  // Create shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &ok);
  if (!ok) {
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    cerr << "[Shader] Error, shader program linking failed: " << infoLog
         << endl;
  }

  // Free shader resources
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() const { glUseProgram(ID); }

int Shader::getAttribLocation(const string &name) const {
  return glGetAttribLocation(ID, name.c_str());
}

void Shader::setBool(const string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setiVec2(const string &name, int x, int y) const {
  glUniform2i(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setiVec3(const string &name, int x, int y, int z) const {
  glUniform3i(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setiVec4(const string &name, int x, int y, int z, int w) const {
  glUniform4i(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const string &name, const mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}