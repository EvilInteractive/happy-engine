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

out vec4 outColor;

uniform sampler2D map;
#if HDR
#if BRIGHTPASS
uniform sampler2D lumMap;
#endif
#endif

void main()
{
    vec3 color = texture(map, texCoord).rgb;
    
#if BRIGHTPASS
    float ex = 0.9f;
#if HDR
    ex = clamp(1.0f / (textureLod(lumMap, vec2(0.5f, 0.5f), 0).r + 0.001f), 0.01f, 1.0f) / 3.0f;
#endif
    color *= ex;
    color -= vec3(1.0f, 1.0f, 1.0f);
    color = vec3(max(color.r, 0.0f), max(color.g, 0.0f), max(color.b, 0.0f));
    color /= ex;
#endif

    outColor = vec4(color, 1.0f);
}


