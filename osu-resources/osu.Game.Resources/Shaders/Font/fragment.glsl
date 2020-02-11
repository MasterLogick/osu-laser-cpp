#version 330 core
out vec4 FragColor;
in vec4 color;
in vec2 TexCoords;

uniform sampler2D textureSampler;

void main()
{
    vec4 t = texture(textureSampler, vec2(TexCoords.s, TexCoords.t));
    if (t.a>0.0f){
        FragColor = vec4(color.r,color.g,color.b, color.a);
    } else {
        FragColor = vec4(0.0f);
    }
}