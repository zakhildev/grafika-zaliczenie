#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Color;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main() {
  vec3 N = normalize(Normal);
  vec3 finalNormal = N;
  
  // Zastosowanie mapowania normalnych (Normal mapping) tylko wtedy, gdy styczna istnieje
  if (length(Tangent) > 0.1) {
    // 1. Pobranie normalnej z tekstury (wartości RGB są w zakresie [0, 1])
    vec3 normalMap = texture(texture_normal1, TexCoords).rgb;
    // 2. Przekształcenie wartości z przestrzeni kolorów [0, 1] na przestrzeń wektorów [-1, 1]
    normalMap = normalMap * 2.0 - 1.0;
    
    // 3. Utworzenie macierzy TBN (Tangent, Bitangent, Normal).
    // Jest to ortogonalna macierz transformacji, która pozwala na przejście 
    // z przestrzeni stycznej (Tangent Space, w której zapisana jest mapa normalnych) 
    // do przestrzeni świata (World Space).
    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    mat3 TBN = mat3(T, B, N);
    
    // 4. Przemnożenie odczytanego wektora przez macierz TBN daje zaktualizowany wektor 
    // normalny powierzchni już w przestrzeni świata, z uwzględnieniem detali z tekstury.
    finalNormal = normalize(TBN * normalMap);
  }

  // Światło otoczenia (Ambient)
  float ambientStrength = 0.25;
  vec3 ambient = ambientStrength * lightColor;
  
  // Światło rozproszone i lustrzane ze źródła 1
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(lightDir, finalNormal), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // Specular - model Phonga dla źródła 1
  float specularStrength = 1.2;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, finalNormal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
  vec3 specMap = vec3(texture(texture_specular1, TexCoords));
  if (length(specMap) < 0.01) specMap = vec3(0.5);
  vec3 specular = specularStrength * spec * lightColor * specMap;
  
  // Światło rozproszone i lustrzane ze źródła 2
  vec3 lightDir2 = normalize(lightPos2 - FragPos);
  float diff2 = max(dot(lightDir2, finalNormal), 0.0);
  vec3 diffuse2 = diff2 * lightColor2;
  
  vec3 reflectDir2 = reflect(-lightDir2, finalNormal);
  float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 64.0);
  vec3 specular2 = specularStrength * spec2 * lightColor2 * specMap;
  
  // Kolor bazowy obiektu z mapy diffuse
  vec3 baseColor = vec3(texture(texture_diffuse1, TexCoords));
  if (length(baseColor) < 0.01) {
    baseColor = Color != vec3(0.0) ? Color : vec3(0.8);
  }
  
  // Równanie oświetlenia: suma oświetlenia z dwóch źródeł
  vec3 result = (ambient + diffuse + diffuse2) * baseColor + specular + specular2;
  FragColor = vec4(result, 1.0);
}