#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
  TexCoords = aTexCoords;
  Normal = aNormal;

  FragPos = vec3(M * vec4(aPos, 1.0));
  gl_Position = P * V * M * vec4(aPos, 1.0);
} 