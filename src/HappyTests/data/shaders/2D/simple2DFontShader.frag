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
in vec2 passTcOffset;
in vec2 passTcScale;

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform vec4 fontColor;

void main()
{
	vec2 tC = vec2(passTexCoord.x, passTexCoord.y);
	float alpha = texture2D(diffuseMap, (tC * passTcScale) + passTcOffset).r;	
	
	outColor = vec4(fontColor.x, fontColor.y, fontColor.z, alpha * fontColor.w);
}