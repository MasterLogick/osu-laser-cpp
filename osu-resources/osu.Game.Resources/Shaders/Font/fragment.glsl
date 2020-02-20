#version 330 core
out vec4 FragColor;
in vec4 color;
in vec2 TexCoords;

uniform sampler2D textureSampler;

void main()
{
    vec4 t = texture(textureSampler, vec2(TexCoords.s, TexCoords.t));
        FragColor = vec4(color.r*t.a,color.g*t.a,color.b*t.a, color.a*t.a);

}