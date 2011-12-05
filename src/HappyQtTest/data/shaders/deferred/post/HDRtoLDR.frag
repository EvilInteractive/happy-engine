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
//    along with HappyEngine.  If not, see <http://www.gnu.org/8licenses/>.
//
//Author:  Bastian Damman
//Created: 14/10/2011

#version 150 core

#include "decode.frag"

noperspective in vec2 texCoord;

out vec4 outColor;

uniform sampler2D hdrMap;

#if BLOOM
uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;
#endif

///////// SSAO //////////
#if SSAO
uniform sampler2D randomNormals;

uniform float radius;
uniform float intensity;
uniform float scale;
uniform float bias;

uniform int passes;
uniform int minIterations;
uniform int maxIterations;

uniform vec4 projParams;
uniform vec2 viewPortSize;

uniform float farZ;
#endif
/////////////////////////

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
    return 1-clamp((endCol.r + endCol.g + endCol.b) / 3.0f, 0.0f, 1.0f); //black-ify
}

/////////////////////////////* SSAO *///////////////////////////////
#if SSAO
vec2 getRandom(in vec2 tc)
{
    return normalize(texture2D(randomNormals, viewPortSize * tc / 64.0f).xy * 2.0f - 1.0f);
}

vec3 getPos(in vec2 tc)
{
    vec2 ndc = tc * 2.0f - 1.0f;
    return getPosition(texture(depthMap, tc).x, ndc, projParams);
}

vec3 getNorm(in vec2 tc)
{
    return decodeNormal(texture(normalMap, tc).xy);
}

float calculateAO(in vec2 tc1, in vec2 tc, in vec3 p, in vec3 cnorm)
{
    vec3 diff = getPos(tc1 + tc) - p;
    vec3 v = normalize(diff);
    float d = length(diff) * scale;
    return max(0.0, dot(cnorm, v) - bias) * (1.0f / (1.0f + d)) * intensity;
}

float renderAO()
{
    vec2 tc = texCoord;
    tc.x = 1.0f - tc.x;

    const vec2 vec[16] = vec2[16](vec2(1,0), vec2(-1,0),
                           vec2(0,1), vec2(0,-1),
                           vec2(0.66,0.66), vec2(-0.66,0.66),
                           vec2(0.66,-0.66), vec2(-0.66,-0.66),
                           vec2(0.33,0.66), vec2(-0.33,0.66),
                           vec2(0.33,-0.66), vec2(-0.33,-0.66),
                           vec2(0.66,0.33), vec2(-0.66,0.33),
                           vec2(0.66,-0.33), vec2(-0.66,-0.33));

    vec3 p = getPos(tc);
    vec3 n = getNorm(tc);
    vec2 rand = getRandom(tc);

    float ao = 0.0f;
    float rad = radius / p.z;

    //uint maxIt = maxIterations;
//
    //if (maxIt > 16)
        //maxIt = 16;
//
    //float weight = p.z / farZ;

    //float iterations = mix(maxIt, minIterations, weight); // LOD

    for (int i = 0; i <= 2; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            vec2 coord1 = reflect(vec[j], rand).xy * (rad / (i * 2));
            vec2 coord2 = vec2( coord1.x * 0.707f - coord1.y * 0.707f,
                                coord1.x * 0.707f + coord1.y * 0.707f );
  
            ao += calculateAO(tc, coord1 * 0.25f, p, n);
            ao += calculateAO(tc, coord2 * 0.5f, p, n);
            ao += calculateAO(tc, coord1 * 0.75f, p, n);
            ao += calculateAO(tc, coord2, p, n);
        }
    }

    ao /= 4 * 4.0f * 2;

    return ao;
}
#endif
///////////////////////////////////////////////////////////////////

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

    //color *= getEdge(normalMap, tex);
    color *= getEdge(depthMap, tex);

    float beginFog = 0.98f;
    float fog = max(0, texture(depthMap, tex).r - beginFog) * (1.0f / (1.0f - beginFog));

    color = color * (1 - fog) + vec3(0.2f, 0.4f, 0.6f) * (fog);

#if SSAO
    color = color * (1.0f - renderAO());
#endif
    
    outColor = vec4(color, 1.0f);
    //float ao = (1.0f - renderAO());
    //outColor = vec4(ao,ao,ao,1.0f);
}