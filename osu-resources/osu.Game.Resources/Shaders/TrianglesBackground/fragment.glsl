#version 330 core
in float colorOut;
out vec4 color;
void main()
{
    color = vec4(1, 1, 1, colorOut);
}