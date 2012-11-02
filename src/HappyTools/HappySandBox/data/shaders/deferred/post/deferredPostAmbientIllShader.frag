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
#pragma optionNV(fastmath on)
//#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

#include "packing/decode.frag"

noperspective in vec2 texCoord;

out vec4 outColor;

struct AmbientLight
{
    vec4 color;
};

struct DirectionalLight
{
    vec4 color;
    vec3 direction;
    vec3 position; //for shadows
    vec2 nearFar;
};


layout(shared) uniform SharedBuffer
{
    vec4 projParams;
};
//layout(std140) uniform LightBuffer
//{
uniform AmbientLight ambLight;
uniform DirectionalLight dirLight;
//};

#if SHADOWS
layout(std140) uniform PerFrameBuffer
{
    mat4 mtxDirLight0;
    mat4 mtxDirLight1;
    mat4 mtxDirLight2;
    mat4 mtxDirLight3;
};

uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
#endif

uniform sampler2D colorIllMap;
uniform sampler2D normalDepthMap;
#if SPECULAR
uniform sampler2D sgMap;
#endif

vec2 PCF9(in sampler2D sampler, in vec2 texCoord)
{
    vec2 color = vec2(0, 0);
    color += textureOffset(sampler, texCoord.xy, ivec2(-1, -1)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 0, -1)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 1, -1)).rg;

    color += textureOffset(sampler, texCoord.xy, ivec2(-1,  0)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 0,  0)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 1,  0)).rg;

    color += textureOffset(sampler, texCoord.xy, ivec2(-1,  1)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 0,  1)).rg;
    color += textureOffset(sampler, texCoord.xy, ivec2( 1,  1)).rg;

    return color / 9.0f;
}

float shadowCheck(in vec3 position, in vec3 lightDir, in vec3 normal, in sampler2D sampler, in mat4 lightMatrix, in float bias)
{
    //float cosLightAngle = dot(lightDir, normal);
    //float normalOffsetScale = clamp(cosLightAngle, 0, 1);
    
    //const float shadowTexelSize = 2.0f / 1024.0f;
    //const float shadowNormalOffset = 3.0f;
    
    //normalOffsetScale *= shadowNormalOffset * shadowTexelSize * length(position - dirLight.position);
    
    //vec3 shadowOffset = normal * normalOffsetScale;

    //vec3 shadowPosition = position + shadowOffset;
    vec3 shadowPosition = position;

    vec4 coord = lightMatrix * vec4(shadowPosition, 1.0f);

    coord.xyz /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1)
        return 1.0f;

    //NDC -> texturespace
    coord.x = coord.x * 0.5f + 0.5f;
    coord.y = coord.y * 0.5f + 0.5f;
        
    //vec2 map = recombinePrecision(texture(sampler, coord.xy));
    vec2 moments = texture(sampler, coord.xy).xy;
    float depth = (length(shadowPosition - dirLight.position) - dirLight.nearFar.x) / dirLight.nearFar.y;

    float p = depth <= moments.x? 1.0f : 0.0f;

    float variance = moments.y - (moments.x * moments.x);
    variance = max(variance, bias);

    float d = depth - moments.x;
    float p_max = pow(variance / (variance + d*d), 50);
    //float p_max = variance / (variance + d*d);


    p_max = max(p, p_max);
    //p_max = clamp((p_max - 0.5f) / (1.0 - 0.5f), 0.0, 1.0);
    
    //return 1 - (min((1 - p_max) * 5, 1));
    return p_max;
}

void main()
{    	
    vec2 ndc = texCoord * 2.0f - 1.0f;
	
#if SPECULAR
    vec4 sg = texture(sgMap, texCoord);    
    if (sg.a < 0.000001f)
        discard;
#endif
    
    vec3 normalDepth = texture(normalDepthMap, texCoord).xyz;       
    vec3 position = getPosition( normalDepth.z, ndc, projParams );

    vec3 lightDir = normalize(dirLight.direction);
        
    vec3 normal = decodeNormal(normalDepth.xy);
    
    //Lambert
    float dotLightNormal = clamp(dot(lightDir, normal), 0.0f, 1.0f);
    
	float ambHalfLambert = clamp(dot(-lightDir, normal), 0.0f, 1.0f) * 0.5f + 0.5f;
    
    //Light
    //vec3 diffuseLight = pow(lambertRamp * halfLambert, vec3(0.3f)) * dirLight.color.rgb * dirLight.color.a;
    vec3 diffuseLight = dotLightNormal * dirLight.color.rgb * dirLight.color.a;
    vec3 ambientLight = ambLight.color.a * ambLight.color.rgb * ambHalfLambert; 
    
    //Shadow
    float shadow = 1;
    //vec3 cascade = vec3(0, 0, 0);
#if SHADOWS
    if (position.z < 20)
    {
        shadow = mix(shadowCheck(position, lightDir, normal, shadowMap0, mtxDirLight0, 0.00001f),
                     shadowCheck(position, lightDir, normal, shadowMap1, mtxDirLight1, 0.0001f),
                     (position.z - 1.0f) / 20.0f);
        //cascade = mix(vec3(1, 0, 0),
        //              vec3(0, 1, 0),
        //              (position.z - 1.0f) / 20.0f);
    }
    else if (position.z < 40)
    {
        shadow = mix(shadowCheck(position, lightDir, normal, shadowMap1, mtxDirLight1, 0.0001f),
                     shadowCheck(position, lightDir, normal, shadowMap2, mtxDirLight2, 0.01f),
                     (position.z - 20.0f) / 20.0f);
        //cascade = mix(vec3(0, 1, 0),
        //              vec3(0, 0, 1),
        //              (position.z - 20.0f) / 20.0f);
    }
    else if (position.z < 60)
    {
        shadow = mix(shadowCheck(position, lightDir, normal, shadowMap2, mtxDirLight2, 0.01f),
                     shadowCheck(position, lightDir, normal, shadowMap3, mtxDirLight3, 0.1f),
                     (position.z - 40.0f) / 20.0f);
        //cascade = mix(vec3(0, 0, 1),
        //              vec3(1, 0, 0),
        //              (position.z - 40.0f) / 20.0f);
    }
    else
    {
        shadow = shadowCheck(position, lightDir, normal, shadowMap3, mtxDirLight3, 0.1f);
        //cascade = mix(vec3(1, 0, 0),
        //              vec3(0, 1, 0),
        //              (position.z - 60.0f) / 20.0f);
    }
#endif

    //Specular
    vec3 spec = vec3(0, 0, 0);
#if SPECULAR
    if (shadow > 0.1f)
    {
        vec3 vCamDir = normalize(-position);
        spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r) * 5.0f * dirLight.color.a * dirLight.color.rgb;
    }
#endif

    //Albedo
    vec4 color = texture(colorIllMap, texCoord);
     
    //Out         
    outColor = vec4(((diffuseLight + spec) * shadow + ambientLight + vec3(color.a, color.a, color.a) * 100) * color.rgb
                        , 1.0f);		
    //outColor *= vec4(cascade, 1);
}
