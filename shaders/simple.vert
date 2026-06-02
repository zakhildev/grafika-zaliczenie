#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
out float DisortionFactor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

// Efekt upicia się
uniform float disortionFactor;
uniform float time;

void main() {
  TexCoords = aTexCoords;
  Normal = mat3(transpose(inverse(M))) *  aNormal;
  DisortionFactor = disortionFactor;

  FragPos = vec3(M * vec4(aPos, 1.0));

  if (disortionFactor > 0.0) {
    // Dodajemy efekt falowania do pozycji wierzchołków
    float disortion = sin(time * 2.0 + aPos.y * 10.0) * disortionFactor * 0.4;
    FragPos.x += disortion;
    FragPos.z += disortion;
  }
  gl_Position = P * V * vec4(FragPos, 1.0);
} 