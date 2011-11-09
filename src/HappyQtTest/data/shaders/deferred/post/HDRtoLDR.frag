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

#if BLOOM
uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;
#endif

uniform sampler2D lumMap;
//uniform float gamma;

float vignette(vec2 pos, float inner, float outer)
{
  float r = length(pos);
  r = 1.0 - smoothstep(inner, outer, r);
  return r;
}

void main()
{
	vec2 tex = vec2(1 - texCoord.x, texCoord.y);

    vec3 color = textureLod(hdrMap, tex, 0.0f).rgb;
    
#if BLOOM
    color += texture(blur0, tex).rgb * 0.5f;  
    color += texture(blur1, tex).rgb * 0.5f;  
    color += texture(blur2, tex).rgb * 1.0f;  
    color += texture(blur3, tex).rgb * 1.0f;  
#endif
  
	float ex = 1.0f / (textureLod(lumMap, vec2(0.5f, 0.5f), 0).r + 0.001f);
	color *= ex / 4.0f;  //0 -> 20
    
	color *= vignette(tex * 2.0f - 1.0f, 0.9f, 2.0f);
	//color = pow(color, vec3(gamma, gamma, gamma));

	outColor = vec4(color, 1.0f);
}