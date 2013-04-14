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
//Created: 14/10/2011

#version 150 core

noperspective in vec2 texCoord;

out float outColor;

uniform sampler2D hdrMap;
uniform sampler2D prevLumMap;

uniform float dTime;

float getLum(in vec3 col)
{
	return (col.r + col.g + col.b) / 3.0f;
}

void main()
{
	float lum = getLum(texture(hdrMap, texCoord).rgb);
	
	outColor = max(0.01f, (lum * dTime + textureLod(prevLumMap, vec2(0.5f, 0.5f), 0).r * (1-dTime)));
}


