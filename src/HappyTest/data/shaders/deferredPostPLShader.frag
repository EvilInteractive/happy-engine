//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 18/08/2011

#version 150 core

in vec2 passTexCoord;

out vec4 outColor;

struct PointLight
{
	vec3 position;
    float multiplier;
    vec3 color;
    float beginAttenuation;
    float endAttenuation;
};

uniform sampler2D colorIllMap;
uniform sampler2D posSpecMap;
uniform sampler2D normGlossMap;

uniform vec3 vCamPos;
uniform PointLight light;

void main()
{
	vec4 colorIll = texture2D(colorIllMap, passTexCoord);
	vec4 posSpec = texture2D(posSpecMap, passTexCoord);
	vec4 normGloss = texture2D(normGlossMap, passTexCoord);

	vec3 normal = normalize(normGloss.xyz);

	vec3 lightDir = light.position - posSpec.xyz;
	float lightDist = length(lightDir);

	if (lightDist > light.endAttenuation) //pixel is too far from light
		discard;

	lightDir /= lightDist;

	float dotLightNormal = dot(lightDir, normal);

	if (dotLightNormal <= 0.0f) //pixel is in selfshadow
		discard;
		
	vec3 vCamDir = normalize(vCamPos - posSpec.xyz);
	float spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), normGloss.a * 100.0f) * posSpec.a);

	float attenuationValue = 1 - max(0, (lightDist - light.beginAttenuation) / (light.endAttenuation - light.beginAttenuation));

	outColor = vec4(
		  (dotLightNormal * colorIll.rgb + vec3(spec, spec, spec)) * 
		  light.color * light.multiplier * attenuationValue, 1.0f);						
}