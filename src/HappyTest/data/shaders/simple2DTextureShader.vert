#version 150 core

in vec2 inPosition;
in vec2 inTexCoord;
in float inAlpha;

out vec2 passTexCoord;
out float passAlpha;

uniform mat4 matWVP;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 0.0f, 1.0f);
	passTexCoord = inTexCoord;
	passAlpha = inAlpha;
}