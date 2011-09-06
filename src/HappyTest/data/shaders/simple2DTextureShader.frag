#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

uniform sampler2D diffuseMap;

void main()
{
	outColor =  texture2D(diffuseMap, passTexCoord);
}