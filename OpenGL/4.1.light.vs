#version 410

layout ( location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
	gl_Position = projection * view *  model * vec4(pos, 1.0);
}