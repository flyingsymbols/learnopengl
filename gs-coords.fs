#version 330 core
out vec4 FragColor;
  
in vec2 frag_tex;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float t;
uniform float k_val;

void main()
{
    vec2 door_coord = frag_tex * 2 - vec2(.5,.5);
    vec2 face_coord = vec2(1.0-frag_tex.x, frag_tex.y) * 2;
    vec2 door2_coord = frag_tex / 128. + vec2(.5,.5) - vec2(1,1)/256.;
    FragColor = mix(mix(
            texture(texture1, door_coord),
            texture(texture2, face_coord),
            k_val
        ),
        texture(texture1, door2_coord),
        sin(t*5)
    );
}
