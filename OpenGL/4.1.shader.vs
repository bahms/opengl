#version 410
layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec2 texLocation;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	texLocation = vec2( texCoord.x, 1.0f - texCoord.y );
}

