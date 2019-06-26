#version 410

in vec3 ourColor;
in vec2 texLocation;

out vec4 outColor;

uniform sampler2D texture0;
uniform sampler2D texture1;


void main()
{
	//outColor = vec4(ourColor, 1.0f);
    outColor = mix( texture(texture0, texLocation) ,  texture(texture1, texLocation), 0.2) ;
    //* vec4(ourColor, 1.0f);
}