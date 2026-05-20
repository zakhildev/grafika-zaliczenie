#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include <atomic>
#include <string>
#include <thread>

using namespace std;
using namespace glm;

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();
  void use() const;
  uint getID() const { return ID; }

  atomic<bool> shouldRecompile{false};
  atomic<bool> watcherStarted{true};
  thread watcherThread;
  void recompile();

  // Uniform setter functions
  void setBool(const string &name, bool value) const;
  void setInt(const string &name, int value) const;
  void setFloat(const string &name, float value) const;
  void setVec2(const string &name, float x, float y) const;
  void setVec3(const string &name, float x, float y, float z) const;
  void setVec4(const string &name, float x, float y, float z, float w) const;
  void setiVec2(const string &name, int x, int y) const;
  void setiVec3(const string &name, int x, int y, int z) const;
  void setiVec4(const string &name, int x, int y, int z, int w) const;
  void setMat4(const string &name, const mat4 &value) const;

  // Attribute location getter
  int getAttribLocation(const string &name) const;

private:
  uint ID;
  string vertexPathStr;
  string fragmentPathStr;
};