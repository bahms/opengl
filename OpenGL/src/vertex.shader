R"(
#version 410

layout ( location = 1 ) in pos

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)"