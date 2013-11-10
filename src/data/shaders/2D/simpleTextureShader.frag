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
//Author: Sebastiaan Sprengers

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
	vec4 color = texture(diffuseMap, passTexCoord);
	vec4 overlay = texture(overlayMap, passTexCoord);
	if (overlay.a > 0.2f)
		color.rgb = overlay.rgb;

	outColor =  color * diffValue + color * 0.3f;
}