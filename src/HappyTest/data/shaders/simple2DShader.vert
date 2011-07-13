#version 150 core
in vec3 inPosition;
in vec3 inColor;
out vec3 passColor;

void main()
{
	gl_Position = vec4(inPosition, 1.0f);
	passColor = inColor;
}