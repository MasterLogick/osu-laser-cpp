#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D yPlaneSampler;
uniform sampler2D uPlaneSampler;
uniform sampler2D vPlaneSampler;

void main()
{
    float r, g, b, y, u, v;
    y = texture2D(yPlaneSampler, TexCoords).r;
    u = texture2D(uPlaneSampler, TexCoords).r;
    v = texture2D(vPlaneSampler, TexCoords).r;

    y = 1.1643*(y-0.0625);
    u = u-0.5;
    v = v-0.5;

    r = y+1.5958*v;
    g = y-0.39173*u-0.81290*v;
    b = y+2.017*u;
    FragColor = vec4(r, g, b, 1.0);
}