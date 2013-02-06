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
uniform vec2 shadowMapInvSize;

vec2 poissonDisk[16] = vec2[]
(
    vec2( -0.94201624, -0.39906216 ), 
    vec2( 0.94558609, -0.76890725 ), 
    vec2( -0.094184101, -0.92938870 ), 
    vec2( 0.34495938, 0.29387760 ), 
    vec2( -0.91588581, 0.45771432 ), 
    vec2( -0.81544232, -0.87912464 ), 
    vec2( -0.38277543, 0.27676845 ), 
    vec2( 0.97484398, 0.75648379 ), 
    vec2( 0.44323325, -0.97511554 ), 
    vec2( 0.53742981, -0.47373420 ), 
    vec2( -0.26496911, -0.41893023 ), 
    vec2( 0.79197514, 0.19090188 ), 
    vec2( -0.24188840, 0.99706507 ), 
    vec2( -0.81409955, 0.91437590 ), 
    vec2( 0.19984126, 0.78641367 ), 
    vec2( 0.14383161, -0.14100790 ) 
);

float PCF(in sampler2D sampler, in vec2 texCoord, in float bias, in float depth)
{
    vec2 mult = shadowMapInvSize * 2;
    float pcf = 0.0f;
    for( int i = 0; i < 16; ++i ) 
    {
        if (depth <= (texture(sampler, texCoord + mult * poissonDisk[i]).r + bias))
        {
            pcf += 1.0f;
        }
    }
    return pcf / 16.0f;
}

float shadowCheck(in vec3 position, in float distFromLight, in sampler2D shadowMap, in mat4 shadowMatrix, in float bias)
{
    vec4 coord = shadowMatrix * vec4(position, 1.0f);

    coord.xy /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1)
        return 1.0f;

    //NDC -> texturespace
    coord.xy = coord.xy * 0.5f + 0.5f;
        
    float depth = (distFromLight - light.beginAttenuation) / (light.endAttenuation - light.beginAttenuation);
    float shadow = PCF(shadowMap, coord.xy, bias, depth);

    return shadow;
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
    shadow = shadowCheck(position, lightDist, shadowMap, shadowMatrix, 0.01f);
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
