#version 150 core

in vec2 passTexCoord;
in float passAlpha;

out vec4 outColor;

uniform sampler2D diffuseMap;

void main()
{
	float alpha = texture2D(diffuseMap, passTexCoord).a - (1.0f - passAlpha);

	if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}

	//outColor =  vec4(texture2D(diffuseMap, passTexCoord).rgb, alpha);

	outColor =  texture2D(diffuseMap, passTexCoord);
}