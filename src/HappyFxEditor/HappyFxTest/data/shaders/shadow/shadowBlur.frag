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
//Created: 20/11/2011

#version 150 core

noperspective in vec2 texCoord;

out vec2 outColor;

uniform sampler2D map;

void main()
{
    vec2 color = vec2(0, 0);
    
#if PASS1
    color += textureOffset(map, texCoord, ivec2(-2, 0)).rg * 0.0625f;
    color += textureOffset(map, texCoord, ivec2(-1, 0)).rg * 0.2500f;
    color += textureOffset(map, texCoord, ivec2( 0, 0)).rg * 0.3750f;
    color += textureOffset(map, texCoord, ivec2( 1, 0)).rg * 0.2500f; 
    color += textureOffset(map, texCoord, ivec2( 2, 0)).rg * 0.0625f;
#endif
#if PASS2
    color += textureOffset(map, texCoord, ivec2(0, -2)).rg * 0.0625f;
    color += textureOffset(map, texCoord, ivec2(0, -1)).rg * 0.2500f;
    color += textureOffset(map, texCoord, ivec2(0,  0)).rg * 0.3750f;
    color += textureOffset(map, texCoord, ivec2(0,  1)).rg * 0.2500f;
    color += textureOffset(map, texCoord, ivec2(0,  2)).rg * 0.0625f;
#endif
            
    outColor = color;
}