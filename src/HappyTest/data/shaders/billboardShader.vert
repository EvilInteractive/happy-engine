#version 150 core

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 passTexCoord;

uniform mat4 matWVP;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 1.0f);
	passTexCoord = inTexCoord;
}