//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 11/08/2011

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