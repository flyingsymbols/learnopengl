#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;

out vec2 frag_tex;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(pos, 1.0);
    frag_tex = tex_coords;
}
