#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Color;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main() {
  vec3 N = normalize(Normal);
  vec3 finalNormal = N;
  
  // Only apply normal mapping if tangent is valid (not 0,0,0)
  if (length(Tangent) > 0.1) {
    vec3 normalMap = texture(texture_normal1, TexCoords).rgb;
    normalMap = normalMap * 2.0 - 1.0;
    
    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    mat3 TBN = mat3(T, B, N);
    finalNormal = normalize(TBN * normalMap);
  }

  // Ambient
  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * lightColor;
  
  // Diffuse
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(lightDir, finalNormal), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // Specular (Blinn-Phong)
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  float spec = pow(max(dot(finalNormal, halfwayDir), 0.0), 32.0);
  vec3 specMap = vec3(texture(texture_specular1, TexCoords));
  if (length(specMap) < 0.01) specMap = vec3(0.5); // Fallback if no spec map
  vec3 specular = specularStrength * spec * lightColor * specMap;
  
  // Base color
  vec3 baseColor = vec3(texture(texture_diffuse1, TexCoords));
  if (length(baseColor) < 0.01) {
      baseColor = Color != vec3(0.0) ? Color : vec3(0.8);
  }
  
  vec3 result = (ambient + diffuse) * baseColor + specular;
  FragColor = vec4(result, 1.0);
}