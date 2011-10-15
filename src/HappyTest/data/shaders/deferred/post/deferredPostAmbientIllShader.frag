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

noperspective in vec2 texCoord;

out vec4 outColor;

struct AmbientLight
{
    float multiplier;
    vec3 color;
};

uniform sampler2D colorIllMap;

uniform AmbientLight light;

void main()
{    
	vec4 color = texture2D(colorIllMap, texCoord);
	
	outColor = vec4(color.rgb * light.color * light.multiplier + color.rgb * color.a * 100.0f, 1.0f);						
}