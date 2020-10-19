#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D textureSampler;
uniform float alpha;

void main()
{
    vec4 e = texture(textureSampler, vec2(TexCoords.s, TexCoords.t));
    if (e.a>0) {
        e.a = alpha;
    }
    FragColor = e;
}