#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float t;
uniform float k_val;

void main()
{
    vec2 door_coord = TexCoord * 2 - vec2(.5,.5);
    vec2 face_coord = vec2(1.0-TexCoord.x, TexCoord.y) * 2;
    vec2 door2_coord = TexCoord / 128. + vec2(.5,.5) - vec2(1,1)/256.;
    FragColor = mix(mix(
            texture(texture1, door_coord),
            texture(texture2, face_coord),
            k_val
        ),
        texture(texture1, door2_coord),
        sin(t*5)
    ); // * vec4(ourColor, 1.0);
}
