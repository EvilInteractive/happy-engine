#version 150 core
in vec2 inPosition;

uniform mat4 matWVP;
uniform float depth;

void main()
{
	gl_Position = matWVP * vec4(inPosition, depth, 1.0f);
}