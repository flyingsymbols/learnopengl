#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // :: Ambient
    vec3 ambient = lightColor * material.ambient;

    // :: Diffuse
    // normalize(Normal) is not really necessary as 
    // Normal is already normalized in the input data
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // :: Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(
        max(dot(viewDir, reflectDir), 0.),
        material.shininess
    );
    vec3 specular = lightColor * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}
