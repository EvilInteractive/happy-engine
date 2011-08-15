#version 150 core

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passWorldPos;

out vec4 outColorIll;
out vec4 outNormalGloss;
out vec4 outPosSpec;

uniform sampler2D diffuseMap;

void main()
{
	outColorIll = vec4(texture2D(diffuseMap, passTexCoord).rgb, 0.0f);
	outPosSpec = vec4(passWorldPos, 0.5f);
	outNormalGloss = vec4(passNormal, 0.5f);
}