#version 330 core
in vec2 position;
in vec2 texCord;
out vec2 TexCoords;
out vec4 color;
layout (std140) uniform Screen
{
    mat4 projection;
    float width;
    float height;
};
uniform vec4 Color;
void main()
{
    color=Color;
    gl_Position = projection*vec4(position, 0.0f, 1.0);
    TexCoords = texCord;
}