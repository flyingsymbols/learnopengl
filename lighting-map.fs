#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
  
uniform vec3 objectColor;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    // :: Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // :: Diffuse
    // normalize(Normal) is not really necessary as 
    // Normal is already normalized in the input data
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.);
    vec3 diffuse = 
        light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // :: Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(
        max(dot(viewDir, reflectDir), 0.),
        material.shininess
    );
    vec3 specular = light.specular * spec
        * vec3(texture(material.specular, TexCoords));

    vec3 emission = 0.2 * texture(material.emission, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular + emission;

	FragColor = vec4(result, 1.0);
}
