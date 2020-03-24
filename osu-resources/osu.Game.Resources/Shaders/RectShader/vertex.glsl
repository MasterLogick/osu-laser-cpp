#version 330 core
in vec2 position;
in vec2 texCord;
out vec2 TexCoords;
layout (std140) uniform Screen
{
    mat4 projection;
    float width;
    float height;
};
uniform float layer;
void main()
{
    gl_Position = projection*vec4(position, layer, 1.0);
    TexCoords = texCord;
}