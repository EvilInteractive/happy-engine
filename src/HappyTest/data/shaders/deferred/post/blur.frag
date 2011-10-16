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
//Created: 16/10/2011

#version 150 core

noperspective in vec2 texCoord;

out vec4 outColor;

uniform sampler2D map;
uniform vec2 texelSize;
uniform float invScale;

void main()
{
    vec3 color = vec3(0, 0, 0);
    
#if PASS1
    color += texture2D(map, invScale*texCoord + vec2(-2.0f * texelSize.x, 0.0f)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(-1.0f * texelSize.x, 0.0f)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(0.0f * texelSize.x, 0.0f)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(1.0f * texelSize.x, 0.0f)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(2.0f * texelSize.x, 0.0f)).rgb;
#endif
#if PASS2
    color += texture2D(map, invScale*texCoord + vec2(0.0f, -2.0f * texelSize.y)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(0.0f, -1.0f * texelSize.y)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(0.0f, 0.0f * texelSize.y)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(0.0f, 1.0f * texelSize.y)).rgb;
    color += texture2D(map, invScale*texCoord + vec2(0.0f, 2.0f * texelSize.y)).rgb;
#endif
        
        
    color /= 5.0f;
    
	outColor = vec4(color, 1.0f);
}