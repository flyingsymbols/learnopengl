#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 xyloc;
uniform vec2 offset;

void main()
{
   xyloc = vec2(aPos.x + offset.x, -aPos.y + offset.y);
   gl_Position = vec4(xyloc, aPos.z, 1.0);
}
