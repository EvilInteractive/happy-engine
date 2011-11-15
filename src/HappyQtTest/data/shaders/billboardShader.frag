#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform vec2 texCoordScale;
uniform float alpha;

void main()
{
	vec4 color = texture2D(diffuseMap, (passTexCoord * texCoordScale));
	outColor =  color;
}