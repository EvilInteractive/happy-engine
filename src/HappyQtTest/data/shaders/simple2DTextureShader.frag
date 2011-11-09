#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform float inAlpha;
uniform vec2 texCoordOffset;
uniform vec2 texCoordScale;

void main()
{
	vec4 color = texture2D(diffuseMap, (passTexCoord * texCoordScale) + texCoordOffset);

	float alpha = color.a - (1.0f - inAlpha);

	if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}

	outColor =  vec4(color.rgb, alpha);
}