#version 330 core
in vec2 localPos;
in vec2 globalPos;
in float alpha;
in float scale;
layout (std140) uniform Screen
{
    mat4 projection;
    float width;
    float height;
};
uniform float x;
uniform float y;
out float colorOut;
void main(){
    colorOut = alpha;
    gl_Position = projection*vec4(localPos*scale+globalPos+vec2(x, y), 0.0f, 1.0f);
}