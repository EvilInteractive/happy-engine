#version 150 core
in vec3 inPosition;

uniform mat4 matVP;
uniform mat4 matW;

void main()
{
	gl_Position = matVP * matW * vec4(inPosition, 1.0f);
}