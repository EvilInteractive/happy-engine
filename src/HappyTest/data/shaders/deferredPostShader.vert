#version 150 core

in vec4 inPosition;
in vec2 inTexCoord;

out vec2 passTexCoord;

void main()
{
	gl_Position = inPosition;
	passTexCoord = inTexCoord;
}