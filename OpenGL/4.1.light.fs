#version 410

uniform vec3 lightCol;
out vec4 outColor;

void main()
{
    outColor = vec4(lightCol, 1.0f);
}