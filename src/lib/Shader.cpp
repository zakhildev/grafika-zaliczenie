#include "../include/Shader.hpp"
#include <GL/glew.h>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <thread>

using namespace std;
using namespace glm;
namespace fs = std::filesystem;

uint compileShader(const char *vertexPath, const char *fragmentPath) {
  uint newID;

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
  newID = glCreateProgram();
  glAttachShader(newID, vertex);
  glAttachShader(newID, fragment);
  glLinkProgram(newID);
  glGetProgramiv(newID, GL_LINK_STATUS, &ok);
  if (!ok) {
    glGetProgramInfoLog(newID, 512, nullptr, infoLog);
    cerr << "[Shader] Error, shader program linking failed: " << infoLog
         << endl;
  }

  // Free shader resources
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return newID;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  this->vertexPathStr = vertexPath;
  this->fragmentPathStr = fragmentPath;
  shouldRecompile = false;
  ID = compileShader(vertexPath, fragmentPath);

  watcherThread = thread([this]() {
    fs::file_time_type vertexLastWriteTime;
    fs::file_time_type fragmentLastWriteTime;

    try {
      vertexLastWriteTime = fs::last_write_time(vertexPathStr);
      fragmentLastWriteTime = fs::last_write_time(fragmentPathStr);
    } catch (fs::filesystem_error &e) {
      cerr << "[Shader] Error reading shader files metadata: " << e.what()
           << endl;
      return;
    }

    while (watcherStarted) {
      this_thread::sleep_for(chrono::milliseconds(500));

      try {
        auto currentVertexWriteTime = fs::last_write_time(vertexPathStr);
        auto currentFragmentWriteTime = fs::last_write_time(fragmentPathStr);

        if ((currentVertexWriteTime != vertexLastWriteTime ||
             currentFragmentWriteTime != fragmentLastWriteTime) &&
            !shouldRecompile) {
          shouldRecompile = true;
          cout << "[Shader] Detected shader file change, marking for "
                  "recompilation."
               << endl;
          vertexLastWriteTime = currentVertexWriteTime;
          fragmentLastWriteTime = currentFragmentWriteTime;
        }
      } catch (fs::filesystem_error &e) {
        cerr << "[Shader] Error reading shader files metadata: " << e.what()
             << endl;
      }
    }
  });
}

Shader::~Shader() {
  watcherStarted = false;
  if (watcherThread.joinable()) {
    watcherThread.join();
  }

  glDeleteProgram(ID);
}

void Shader::recompile() {
  glDeleteProgram(ID);
  ID = compileShader(vertexPathStr.c_str(), fragmentPathStr.c_str());
  shouldRecompile = false;
}

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