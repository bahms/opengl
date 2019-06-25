#version 410
layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in vec2 texCoord;

out vec3 ourColor;
out vec2 texLocation;

void main()
{
	gl_Position = vec4(pos, 1.0);
	ourColor = color;
	texLocation = vec2( texCoord.x, 1.0f - texCoord.y );
}

