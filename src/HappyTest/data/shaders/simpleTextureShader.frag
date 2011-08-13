#version 150 core

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passWorldPos;

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform sampler2D overlayMap;

void main()
{
	vec3 normal = normalize(passNormal);

	vec3 lightDir = normalize(vec3(0, 5, 0) - passWorldPos);
	float diffValue = clamp(dot(normal, lightDir), 0.0f, 1.0f);
	vec4 color = texture2D(diffuseMap, passTexCoord);
	vec4 overlay = texture2D(overlayMap, passTexCoord);
	if (overlay.a > 0.2f)
		color.rgb = overlay.rgb;

	outColor =  color * diffValue + color * 0.3f;
}