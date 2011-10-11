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

#include "decode.frag"

in vec3 passPos;

out vec4 outColor;

struct SpotLight
{
	vec3 position;
    float multiplier;
    vec3 direction;
    float beginAttenuation;
	vec3 color;
    float endAttenuation;
	float cosCutoff;
};

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D sgiMap;
uniform sampler2D depthMap;

uniform vec4 projParams;
uniform SpotLight light;

void main()
{
    vec2 ndc = passPos.xy / passPos.z;
    vec2 texCoord = ndc * 0.5f + 0.5f;
    	
	vec3 position = getPosition( texture2D(depthMap, texCoord).x, ndc, projParams );
	
	vec3 lightDir = light.position - position;
	float lightDist = length(lightDir);

	if (lightDist > light.endAttenuation) //pixel is too far from light
		discard;

	lightDir /= lightDist;
	
	float spot = dot(light.direction, lightDir);

	if (spot <= light.cosCutoff)
		discard;

	float maxFalloffSpot = light.cosCutoff * -0.25f + 0.25f; //optimized to a MAD operation (1 - light.cosCutoff) * 0.25f
	float maxInnerSpot = light.cosCutoff + maxFalloffSpot;
	spot = min(0, (spot - maxInnerSpot)) / (maxFalloffSpot) + 1;
	
	vec3 normal = getNormal(texture2D(normalMap, texCoord).xy);
	float dotLightNormal = dot(lightDir, normal);

	if (dotLightNormal <= 0.0f) //pixel is in selfshadow
		discard; 
	
	vec4 sgi = texture2D(sgiMap, texCoord);	
	vec3 vCamDir = normalize(-position);
	float spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sgi.b * 100.0f) * sgi.r);

	float attenuationValue = 1 - max(0, (lightDist - light.beginAttenuation) / (light.endAttenuation - light.beginAttenuation));
	
	vec4 color = texture2D(colorMap, texCoord);
	outColor = vec4(
		  (dotLightNormal * color.rgb + vec3(spec, spec, spec)) 
		  * light.color * light.multiplier * attenuationValue * spot, 1.0f);						
}