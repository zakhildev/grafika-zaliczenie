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
  // Przekształcenie pozycji wierzchołka z przestrzeni lokalnej (obiektu) do przestrzeni świata.
  // Używamy macierzy modelu (M). Współrzędna w = 1.0 pozwala na uwzględnienie translacji (przesunięcia).
  FragPos = vec3(M * vec4(aPos, 1.0));
  
  // Przekształcenie wektora normalnego do przestrzeni świata.
  // Używamy mat3(M) (lewego górnego fragmentu macierzy modelu 3x3), aby odrzucić 
  // informacje o translacji, ponieważ wektor normalny reprezentuje tylko kierunek.
  Normal = mat3(M) * aNormal;
  
  TexCoords = aTexCoords;
  Color = aColor;
  
  // Przekształcenie wektorów stycznego (Tangent) i binormalnego (Bitangent) do przestrzeni świata.
  // Podobnie jak wektor normalny, odrzucamy z macierzy M informacje o translacji.
  Tangent = mat3(M) * aTangent;
  Bitangent = mat3(M) * aBitangent;
  
  // Obliczenie końcowej pozycji wierzchołka w przestrzeni obcinania (clip space).
  // Operacje mnożenia wektorów kolumnowych przez macierze w GLSL czyta się od prawej do lewej:
  // 1. M * vec4(...) -> Przejście do przestrzeni świata (World Space)
  // 2. V * ...       -> Przejście do przestrzeni widoku/kamery (View Space)
  // 3. P * ...       -> Przejście do przestrzeni rzutowania (Projection / Clip Space)
  gl_Position = P * V * M * vec4(aPos, 1.0);
}