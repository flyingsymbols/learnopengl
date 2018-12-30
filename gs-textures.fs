#version 330 core

out vec4 FragColor;
in vec2 xyloc;
uniform vec4 ourColor;

void main()
{
   FragColor = vec4(ourColor.x + xyloc.x, ourColor.y + xyloc.y, ourColor.z, ourColor.w);
}
