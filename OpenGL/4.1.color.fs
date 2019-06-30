#version 410

uniform vec3 objectColor;
uniform vec3 lightColor;
out vec4 outColor;

void main()
{
	outColor = vec4(objectColor * lightColor, 1.0f);
}