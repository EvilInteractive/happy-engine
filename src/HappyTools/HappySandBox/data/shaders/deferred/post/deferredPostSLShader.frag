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

#include "packing/decode.frag"

in vec3 passPos;
out vec4 outColor;

struct SpotLight
{
    vec3 position;
    float multiplier;
    vec3 direction;
    float beginAttenuation;
    vec3 color;
    float endAttenuation;
    float cosCutoff;
};

layout(shared) uniform SharedBuffer
{
    vec4 projParams;
};

uniform SpotLight light;

uniform sampler2D colorIllMap;
uniform sampler2D normalDepthMap;
#if SPECULAR
uniform sampler2D sgMap;
#endif

#if SHADOWS
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;   // = shadowViewProjMatrix * invViewMatrix => transform viewPosition to world -> world to shadowViewProj

float PCF9(in sampler2D sampler, in vec2 texCoord)
{
    float depth = 0;
    depth += textureOffset(sampler, texCoord, ivec2(-1, -1)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 0, -1)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 1, -1)).r;

    depth += textureOffset(sampler, texCoord, ivec2(-1,  0)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 0,  0)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 1,  0)).r;

    depth += textureOffset(sampler, texCoord, ivec2(-1,  1)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 0,  1)).r;
    depth += textureOffset(sampler, texCoord, ivec2( 1,  1)).r;

    return depth / 9.0f;
}

float shadowCheck(in vec3 position, in float distFromLight, in sampler2D shadowMap, in mat4 shadowMatrix, in float bias)
{
    vec4 coord = shadowMatrix * vec4(position, 1.0f);

    coord.xy /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1)
        return 1.0f;

    //NDC -> texturespace
    coord.xy = coord.xy * 0.5f + 0.5f;
        
    float shadowDepth = PCF9(shadowMap, coord.xy) + bias;
    float depth = (distFromLight - light.beginAttenuation) / (light.endAttenuation - light.beginAttenuation);

    return depth <= shadowDepth? 1.0f : 0.0f;
}

#endif

void main()
{
    vec2 ndc = passPos.xy / passPos.z;
    vec2 texCoord = ndc * 0.5f + 0.5f;
     
    vec3 normalDepth = texture(normalDepthMap, texCoord).xyz;   
    vec3 position = getPosition( normalDepth.z, ndc, projParams );
    
    vec3 lightDir = light.position - position;
    float lightDist = length(lightDir);

    // Check end attenuation
    if (lightDist > light.endAttenuation) //pixel is too far from light
        discard;

    // Normalize LightDir
    lightDir /= lightDist;
    
    // Check spot FOV
    float spot = dot(-light.direction, lightDir);
    if (spot < light.cosCutoff)
        discard;

    // Check shadow	
    float shadow = 1;

#if SHADOWS
    shadow = shadowCheck(position, lightDist, shadowMap, shadowMatrix, 0.001f);
    if (shadow < 0.1f)
        discard;
#endif
    
    float maxFalloffSpot = -light.cosCutoff * 0.25f + 0.25f; //optimized to a MAD operation (1 - light.cosCutoff) * 0.25f
    float maxInnerSpot = light.cosCutoff + maxFalloffSpot;
    spot = min(0, (spot - maxInnerSpot)) / (maxFalloffSpot) + 1;
    
    vec3 normal = decodeNormal(normalDepth.xy);

    float dotLightNormal = dot(lightDir, normal);

    if (dotLightNormal <= 0.0f) //pixel is in selfshadow
        discard; 
    
    float spec = 0.0f;
#if SPECULAR
    vec4 sg = texture(sgMap, texCoord);	
    vec3 vCamDir = normalize(-position);
    spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r);
#endif

    float attenuationValue = 1 - max(0, (lightDist - light.beginAttenuation) / (light.endAttenuation - light.beginAttenuation));
    
    vec4 color = texture(colorIllMap, texCoord);
    outColor = vec4(
          (dotLightNormal * color.rgb + vec3(spec, spec, spec)) *
          light.color * light.multiplier * attenuationValue * spot * shadow, 1.0f);						
}
