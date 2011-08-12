#version 150 core

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passWorldPos;

out vec4 outColor;

uniform sampler2D diffuseMap;

void main()
{
	vec3 normal = normalize(passNormal);

	vec3 lightDir = normalize(vec3(0, 5, 0) - passWorldPos);
	float diffValue = clamp(dot(normal, lightDir), 0.0f, 1.0f);
	vec4 color = texture2D(diffuseMap, passTexCoord);

	outColor =  color * diffValue + color * 0.3f;
}