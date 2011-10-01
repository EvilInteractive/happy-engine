#version 150 core
in vec2 inPosition;
in vec4 inColor;
out vec4 passColor;

uniform mat4 matWVP;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 0.1f, 1.0f);
	passColor = inColor;
}