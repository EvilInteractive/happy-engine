#version 150 core
in vec3 inPosition;
in vec3 inColor;
out vec3 passColor;

uniform mat4 matWVP;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 1.0f);
	passColor = (inColor + 1.0f) / 2.0f;
}