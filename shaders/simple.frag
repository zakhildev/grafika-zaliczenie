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
  // Symuluje globalne rozproszenie światła. Mnożenie stałej (0.35) przez kolor światła
  // daje podstawowy poziom oświetlenia w cieniu.
  float ambientStrength = 0.35;
  vec3 ambient = ambientStrength * lightColor;
  
  // Światło rozproszone (Diffuse) - model Lamberta
  // 1. Obliczenie wektora kierunku światła od danego fragmentu do źródła światła (lightPos).
  vec3 lightDir = normalize(lightPos - FragPos);
  // 2. Iloczyn skalarny (dot product) dwóch znormalizowanych wektorów daje cosinus kąta między nimi.
  // Jeśli wektory są równoległe (kąt 0), wynik to 1 (maksymalne oświetlenie).
  // Funkcja max(..., 0.0) upewnia się, że nie otrzymamy ujemnego oświetlenia, gdy powierzchnia jest odwrócona tyłem do światła (kąt > 90 stopni).
  float diff = max(dot(lightDir, finalNormal), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // Światło lustrzane (Specular) - model Blinna-Phonga
  float specularStrength = 0.5;
  // 1. Wektor z fragmentu w stronę kamery (obserwatora).
  vec3 viewDir = normalize(viewPos - FragPos);
  // 2. Wektor połówkowy (halfway vector) - wektor znajdujący się dokładnie w połowie
  // drogi między wektorem kierunku światła a wektorem patrzenia.
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  // 3. Obliczenie natężenia odbicia: cosinus kąta między normalną powierzchni a wektorem połówkowym.
  // Podniesienie wyniku do potęgi (tutaj 32) powoduje, że odblask staje się mniejszy i bardziej ostry (tzw. shininess).
  float spec = pow(max(dot(finalNormal, halfwayDir), 0.0), 32.0);
  vec3 specMap = vec3(texture(texture_specular1, TexCoords));
  if (length(specMap) < 0.01) specMap = vec3(0.5); // Wartość domyślna w przypadku braku tekstury specular
  // Ostateczny kolor odbicia to wymnożona siła odblasku, obliczona wartość potęgowa, kolor światła i próbka z mapy odblasków.
  vec3 specular = specularStrength * spec * lightColor * specMap;
  
  // Kolor bazowy obiektu z mapy diffuse
  vec3 baseColor = vec3(texture(texture_diffuse1, TexCoords));
  if (length(baseColor) < 0.01) {
    baseColor = Color != vec3(0.0) ? Color : vec3(0.8);
  }
  
  // Równanie oświetlenia: 
  // Naświetlenie otoczenia i rozproszone są dodawane do siebie i modulują (mnożą) bazowy kolor materiału.
  // Na koniec do całości dodawane jest oświetlenie lustrzane (odblaski nakładają się addytywnie).
  vec3 result = (ambient + diffuse) * baseColor + specular;
  FragColor = vec4(result, 1.0);
}