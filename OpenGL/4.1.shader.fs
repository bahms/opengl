#version 410
in vec3 ourColor;
out vec4 outColor;
void main()
{
	outColor = vec4(ourColor, 1.0f);
}