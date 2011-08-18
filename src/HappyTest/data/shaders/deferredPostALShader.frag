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

struct AmbientLight
{
	vec3 position;
    float multiplier;
    vec3 color;
	float range;
};

uniform sampler2D colorIllMap;
uniform sampler2D posSpecMap;

uniform AmbientLight light;

void main()
{
	vec4 colorIll = texture2D(colorIllMap, passTexCoord);
	vec4 posSpec = texture2D(posSpecMap, passTexCoord);

	float lightDist = length(light.position - posSpec.xyz);

	if (lightDist > light.range) //pixel is too far from light
		discard;

	outColor = vec4(colorIll.rgb * light.color * light.multiplier, 1.0f);						
}