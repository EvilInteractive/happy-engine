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

#include "decode.frag"

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
};

layout(shared) uniform SharedBuffer
{
    vec4 projParams;
};
layout(packed) uniform PerFrameBuffer
{
    mat4 mtxDirLight0;
    mat4 mtxDirLight1;
    mat4 mtxDirLight2;
    mat4 mtxDirLight3;
};
layout(packed) uniform LightBuffer
{
    AmbientLight ambLight;
    DirectionalLight dirLight;
};

uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;

uniform sampler2D colorIllMap;
uniform sampler2D normalMap;
uniform sampler2D sgMap;
uniform sampler2D depthMap;

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

float shadowCheck(in vec3 position, in sampler2D sampler, in mat4 lightMatrix)
{
    vec4 coord = lightMatrix * vec4(position, 1.0f);
    coord.xyz /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1)
        return 1.0f;

    //NDC -> texturespace
    coord.x = coord.x * 0.5f + 0.5f;
    coord.y = coord.y * 0.5f + 0.5f;
    //coord.z = coord.y * 0.5f + 0.5f;
    
    //float bias = 0.001f;

    //vec2 map = PCF9(sampler, coord.xy);
    vec2 map = texture(sampler, coord.xy).rg;


    float fAvgZ = map.x;
    float fAvgZ2 = map.y;

    if (coord.z <= fAvgZ) return 1.0f;
    if (coord.z >= 1.0f) return 0.0f;

    float variance = fAvgZ2 - (fAvgZ * fAvgZ);
    variance = min(max(variance, 0.0f) + 0.00003f, 1.0f);

    float mean = fAvgZ;
    float d = coord.z - mean;
    
    return pow(variance / (variance + d*d), 25);
}

void main()
{    
    vec2 ndc = texCoord * 2.0f - 1.0f;
        
    vec3 position = getPosition( texture(depthMap, texCoord).x, ndc, projParams );

    vec3 lightDir = dirLight.direction;
        
    vec3 normal = decodeNormal(texture(normalMap, texCoord).xy);
    
    float dotLightNormal = dot(lightDir, normal) * 0.5f + 0.5f;
    //dotLightNormal *= dotLightNormal;
    dotLightNormal = max(0.0f, dotLightNormal);
    
    vec4 sg = texture(sgMap, texCoord);	
    vec3 vCamDir = normalize(-position);
    float spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r);
    
    vec4 color = texture(colorIllMap, texCoord);

    vec3 dirColor = (dotLightNormal * color.rgb + vec3(spec, spec, spec) * 5.0f) * dirLight.color.rgb;
    
    vec3 testColor = vec3(1, 1, 1);

    if (position.z < 25)
    {
        //testColor = vec3(1, 0, 1);
        dirColor *= shadowCheck(position, shadowMap0, mtxDirLight0);
    }
    else if (position.z < 50)
    {
        //testColor = vec3(0, 1, 0);
        dirColor *= shadowCheck(position, shadowMap1, mtxDirLight1);
    }
    else if (position.z < 100)
    {
        //testColor = vec3(0, 0, 1);
        dirColor *= shadowCheck(position, shadowMap2, mtxDirLight2);
    }
    else
    {
        //testColor = vec3(0, 1, 1);
        dirColor *= shadowCheck(position, shadowMap3, mtxDirLight3);
    }
    
    //float steps = 1.0f / 16.0f;
    //color.r = int(color.r / steps) * steps;
    //color.g = int(color.g / steps) * steps;
    //color.b = int(color.b / steps) * steps;
//
    //dirColor.r = int(dirColor.r / steps) * steps;
    //dirColor.g = int(dirColor.g / steps) * steps;
    //dirColor.b = int(dirColor.b / steps) * steps;
    

    outColor = vec4((color.rgb * ambLight.color.a * ambLight.color.rgb + 
                    color.rgb * color.a * 10.0f + 
                    dirColor.rgb * dirLight.color.a)*testColor, 0.0f);						
}