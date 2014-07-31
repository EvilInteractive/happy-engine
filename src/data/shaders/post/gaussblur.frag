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


void main()
{
    vec3 color = vec3(0, 0, 0);
    
    // Normalized in 1D
    /*
    0.05309227f;
    0.087110729f;
    0.124071462f;
    0.153402364f;
    0.164646348f;
    0.153402364f;
    0.124071462f;
    0.087110729f;
    0.05309227f;
    */
    
    // Normalized in 2D
    /*
    0.008741448f;
    0.014342463f;
    0.020427913f;
    0.025257139f;
    0.02710842f;;
    0.025257139f;
    0.020427913f;
    0.014342463f;
    0.008741448f;
    */

#if PASS1
    color += textureOffset(map, texCoord, ivec2(-4, 0)).rgb * 0.05309227f;
    color += textureOffset(map, texCoord, ivec2(-3, 0)).rgb * 0.087110729f;
    color += textureOffset(map, texCoord, ivec2(-2, 0)).rgb * 0.124071462f;
    color += textureOffset(map, texCoord, ivec2(-1, 0)).rgb * 0.153402364f;
    color += textureOffset(map, texCoord, ivec2( 0, 0)).rgb * 0.164646348f;
    color += textureOffset(map, texCoord, ivec2( 1, 0)).rgb * 0.153402364f;
    color += textureOffset(map, texCoord, ivec2( 2, 0)).rgb * 0.124071462f;
    color += textureOffset(map, texCoord, ivec2( 3, 0)).rgb * 0.087110729f;
    color += textureOffset(map, texCoord, ivec2( 4, 0)).rgb * 0.05309227f;
#endif
#if PASS2
    color += textureOffset(map, texCoord, ivec2(0, -4)).rgb * 0.05309227f;
    color += textureOffset(map, texCoord, ivec2(0, -3)).rgb * 0.087110729f;
    color += textureOffset(map, texCoord, ivec2(0, -2)).rgb * 0.124071462f;
    color += textureOffset(map, texCoord, ivec2(0, -1)).rgb * 0.153402364f;
    color += textureOffset(map, texCoord, ivec2(0,  0)).rgb * 0.164646348f;
    color += textureOffset(map, texCoord, ivec2(0,  1)).rgb * 0.153402364f;
    color += textureOffset(map, texCoord, ivec2(0,  2)).rgb * 0.124071462f;
    color += textureOffset(map, texCoord, ivec2(0,  3)).rgb * 0.087110729f;
    color += textureOffset(map, texCoord, ivec2(0,  4)).rgb * 0.05309227f;
#endif
            
    outColor = vec4(color, 1.0f);
}