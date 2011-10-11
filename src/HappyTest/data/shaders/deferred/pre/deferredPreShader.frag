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
//Author: Bastian Damman

#version 150 core

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passTangent;

out vec4 outColor;
out vec2 outNormal;
out vec4 outSGI;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specGlossIllMap;

vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)
{
	//NormalMap
	tangent = normalize(tangent);
	normal = normalize(normal);

	tangent -= dot(tangent, normal) * normal;
	vec3 binormal = normalize(cross(tangent, normal));

	mat3 assenstelsel = mat3(binormal, tangent, normal);

	vec3 xyz = vec3(rgb.x * 2 - 1, (1-rgb.y) * 2 - 1, rgb.z * 2 - 1);

	return normalize(assenstelsel * xyz);
}

void main()
{
	vec4 color = texture2D(diffuseMap, passTexCoord);
	vec4 normal = texture2D(normalMap, passTexCoord);
	vec4 specGlossIll = texture2D(specGlossIllMap, passTexCoord);

	outColor = color.rgba;

	vec3 n = calcNormal(passNormal, passTangent, normal.rgb);
	vec2 encNormal = normalize(n.xy) * sqrt(-n.z * 0.5f + 0.5f);
	encNormal = encNormal * 0.5f + 0.5f;
	outNormal = encNormal;

	outSGI = vec4(specGlossIll.rgb, 0);
}