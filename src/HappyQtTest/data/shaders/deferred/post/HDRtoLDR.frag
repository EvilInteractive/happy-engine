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

uniform sampler2D normalMap;
uniform sampler2D depthMap;

float edgeMult1 = 5.0f;
float edgeMult2 = 10.0f;

float vignette(vec2 pos, float inner, float outer)
{
    float r = length(pos);
    r = 1.0 - smoothstep(inner, outer, r);
    return r;
}

float getEdge(in sampler2D map, in vec2 texCoord)
{
    vec3 sumX = vec3(0, 0, 0);
    vec3 sumY = vec3(0, 0, 0); 
    
    vec3 c00 = textureOffset(map, texCoord, ivec2(-1, -1)).rgb;
    vec3 c01 = textureOffset(map, texCoord, ivec2(0, -1)).rgb;
    vec3 c02 = textureOffset(map, texCoord, ivec2(1, -1)).rgb;

    vec3 c10 = textureOffset(map, texCoord, ivec2(-1, 0)).rgb;
    vec3 c12 = textureOffset(map, texCoord, ivec2(1, 0)).rgb;

    vec3 c20 = textureOffset(map, texCoord, ivec2(-1, 1)).rgb;
    vec3 c21 = textureOffset(map, texCoord, ivec2(0, 1)).rgb;
    vec3 c22 = textureOffset(map, texCoord, ivec2(1, 1)).rgb;

    sumX -= c00 * edgeMult1;
    sumX -= c01 * edgeMult2;
    sumX -= c02 * edgeMult1;
    sumX += c20 * edgeMult1;
    sumX += c21 * edgeMult2;
    sumX += c22 * edgeMult1;

    sumY -= c00 * edgeMult1;
    sumY += c02 * edgeMult1;
    sumY -= c10 * edgeMult2;
    sumY += c12 * edgeMult2;
    sumY -= c20 * edgeMult1;
    sumY += c22 * edgeMult1;

    vec3 endCol = sumX * sumX + sumY * sumY;
    return 1-clamp((endCol.r + endCol.g + endCol.b) / 3.0f, 0.0f, 1.0f); //back-ify
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

    color *= getEdge(normalMap, tex);
    color *= getEdge(depthMap, tex);

    float beginFog = 0.98f;
    float fog = max(0, texture(depthMap, tex).r - beginFog) * (1.0f / (1.0f - beginFog));


    color = color * (1 - fog) + vec3(0.2f, 0.4f, 0.6f) * (fog);
    
    outColor = vec4(color, 1.0f);
}