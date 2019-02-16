#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec3 ambient = 0.1 * lightColor * objectColor;
	FragColor = vec4(ambient, 1.0);
}
