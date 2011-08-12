#version 150 core

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passWorldPos;

uniform mat4 matWVP;
uniform mat4 matWorld;

void main()
{
	gl_Position = matWVP * vec4(inPosition, 1.0f);
	passTexCoord = inTexCoord;
	passNormal = (matWorld * vec4(inNormal, 0.0f)).xyz;
	passWorldPos = (matWorld * vec4(inPosition, 1.0f)).xyz;
}