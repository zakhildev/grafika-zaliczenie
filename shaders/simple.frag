#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in float DisortionFactor;

out vec4 FragColor;

uniform vec3 ambientColor;
uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
  // Ambient
  float ambientStrength = 0.5;
  vec3 ambient = ambientStrength * ambientColor;

  // Diffuse
  vec3 norm = normalize(Normal);

  // Light 1
  vec3 lightDir1 = normalize(lightPos1 - FragPos);
  float diff1 = max(dot(norm, lightDir1), 0.0);
  vec3 diffuse1 = diff1 * lightColor1;

  // Light 2
  vec3 lightDir2 = normalize(lightPos2 - FragPos);
  float diff2 = max(dot(norm, lightDir2), 0.0);
  vec3 diffuse2 = diff2 * lightColor2;

  // Specular
  float specularStrength = 1.5;
  vec3 viewDir = normalize(viewPos - FragPos);

  // Light 1
  vec3 halfwayDir1 = normalize(lightDir1 + viewDir);
  float spec1 = pow(max(dot(norm, halfwayDir1), 0.0), 64.0);
  vec3 specular1 = specularStrength * spec1 * vec3(texture(texture_specular1, TexCoords)) * lightColor1;
  
  // Light 2
  vec3 halfwayDir2 = normalize(lightDir2 + viewDir);
  float spec2 = pow(max(dot(norm, halfwayDir2), 0.0), 64.0);
  vec3 specular2 = specularStrength * spec2 * vec3(texture(texture_specular1, TexCoords)) * lightColor2;

  vec3 lightning = ambient + diffuse1 + diffuse2 + specular1 + specular2;
  vec3 result = lightning * vec3(texture(texture_diffuse1, TexCoords));
  FragColor = vec4(result + (DisortionFactor * 2.0f), 1);
}