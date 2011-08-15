#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

uniform sampler2D colorIllMap;
uniform sampler2D posSpecMap;
uniform sampler2D normGlossMap;

uniform vec3 vCamPos;

void main()
{
	vec4 colorIll = texture2D(colorIllMap, passTexCoord);
	vec4 posSpec = texture2D(posSpecMap, passTexCoord);
	vec4 normGloss = texture2D(normGlossMap, passTexCoord);

	vec3 normal = normalize(normGloss.xyz);

	vec3 lightDir = vec3(-1, 0, -1) - posSpec.xyz;
	float lightDist = length(lightDir);

	//if (lightDist > 10.0f)
	//	discard;

	lightDir /= lightDist;

	float dotLightNormal = max(0, dot(lightDir, normal));
	float diffuseValue = dotLightNormal * 2.0f;

	vec3 vCamDir = normalize(vCamPos - posSpec.xyz);
	float spec = max(0, pow(dot(reflect(lightDir, normal), vCamDir), normGloss.a * 25.0f) * posSpec.a);

	outColor = vec4(
		(colorIll.rgb * diffuseValue + vec3(1, 1, 1) * spec) * 
		(max(0, (10 - lightDist) / 10.0f)) + 
		colorIll.rgb * 0.3f, 1.0f);
}