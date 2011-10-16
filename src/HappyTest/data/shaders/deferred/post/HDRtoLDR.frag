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

uniform sampler2D hdrMap;

uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;

uniform float exposure;
uniform float gamma;

float vignette(vec2 pos, float inner, float outer)
{
  float r = length(pos);
  r = 1.0 - smoothstep(inner, outer, r);
  return r;
}

void main()
{
    vec3 color = texture2D(hdrMap, texCoord).rgb;
    
    color += texture2D(blur0, texCoord).rgb * 0.5f;  
    color += texture2D(blur1, texCoord).rgb * 0.5f;  
    color += texture2D(blur2, texCoord).rgb * 1.0f;  
    color += texture2D(blur3, texCoord).rgb * 1.0f;    
    color *= exposure;
    
	color *= vignette(texCoord * 2.0f - 1.0f, 0.7f, 1.5f);
	color = pow(color, vec3(gamma, gamma, gamma));

	outColor = vec4(color, 1.0f);
}