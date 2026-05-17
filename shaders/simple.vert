#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Color;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
  FragPos = vec3(M * vec4(aPos, 1.0));
  Normal = mat3(M) * aNormal;
  TexCoords = aTexCoords;
  Color = aColor;
  Tangent = mat3(M) * aTangent;
  Bitangent = mat3(M) * aBitangent;
  
  gl_Position = P * V * M * vec4(aPos, 1.0);
}