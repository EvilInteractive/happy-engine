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

in vec4 passPos;

out vec4 outColor;

struct DirectionalLight
{
	vec3 direction;
    vec3 color;
    float multiplier;
};

uniform sampler2D colorIllMap;
uniform sampler2D posSpecMap;
uniform sampler2D normGlossMap;

uniform vec3 vCamPos;
uniform DirectionalLight light;

void main()
{
    vec2 texCoord = ((passPos.xy / passPos.w) + 1.0f) * 0.5f;
    
	vec4 colorIll = texture2D(colorIllMap, texCoord);
	vec4 posSpec = texture2D(posSpecMap, texCoord);
	vec4 normGloss = texture2D(normGlossMap, texCoord);

	vec3 normal = normalize(normGloss.xyz);
	
	float dotLightNormal = dot(light.direction, normal);

	if (dotLightNormal <= 0.0f) //pixel is in selfshadow
		discard;
		
	vec3 vCamDir = normalize(vCamPos - posSpec.xyz);
	float spec = max(0, pow(dot(reflect(-light.direction, normal), vCamDir), normGloss.a * 100.0f) * posSpec.a);

	outColor = vec4((dotLightNormal * colorIll.rgb + vec3(spec, spec, spec)) * light.color * light.multiplier, 1.0f);						
}