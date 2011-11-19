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

uniform sampler2D colorIllMap;
uniform sampler2D normalMap;
uniform sampler2D sgMap;
uniform sampler2D depthMap;

uniform vec4 projParams;

uniform AmbientLight ambLight;
uniform DirectionalLight dirLight;

uniform mat4 mtxDirLight0;
uniform mat4 mtxDirLight1;
uniform mat4 mtxDirLight2;
uniform mat4 mtxDirLight3;
uniform sampler2DShadow shadowMap0;
uniform sampler2DShadow shadowMap1;
uniform sampler2DShadow shadowMap2;
uniform sampler2DShadow shadowMap3;

float shadowCheck(in vec3 position, in sampler2DShadow sampler, in mat4 lightMatrix, in float bias)
{
    vec4 coord = lightMatrix * vec4(position, 1.0f);
    coord.xyz /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1 ||
        coord.z < 0)
        return 0.0f;

    //NDC -> texturespace
    coord.x = (coord.x + 1.0f) / 2.0f;
    coord.y = (coord.y + 1.0f) / 2.0f;
    coord.z = (coord.z + 1.0f) / 2.0f - bias;
    
    float shadow = 0;
	shadow += textureOffset(sampler, coord.xyz, ivec2(-2, -2));
	shadow += textureOffset(sampler, coord.xyz, ivec2(-1, -2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 0, -2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 1, -2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 2, -2));
	
	shadow += textureOffset(sampler, coord.xyz, ivec2(-2, -1));
	shadow += textureOffset(sampler, coord.xyz, ivec2(-1, -1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 0, -1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 1, -1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 2, -1));
	
	shadow += textureOffset(sampler, coord.xyz, ivec2(-2, 0));
	shadow += textureOffset(sampler, coord.xyz, ivec2(-1, 0));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 0, 0));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 1, 0));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 2, 0));
	
	shadow += textureOffset(sampler, coord.xyz, ivec2(-2, 1));
	shadow += textureOffset(sampler, coord.xyz, ivec2(-1, 1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 0, 1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 1, 1));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 2, 1));
	
	shadow += textureOffset(sampler, coord.xyz, ivec2(-2, 2));
	shadow += textureOffset(sampler, coord.xyz, ivec2(-1, 2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 0, 2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 1, 2));
	shadow += textureOffset(sampler, coord.xyz, ivec2( 2, 2));
    
    shadow /= 25;

    return shadow;
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
        dirColor *= shadowCheck(position, shadowMap0, mtxDirLight0, 0.0025f);
    }
    else if (position.z < 50)
    {
        //testColor = vec3(0, 1, 0);
        dirColor *= shadowCheck(position, shadowMap1, mtxDirLight1, 0.005f);
    }
    else if (position.z < 100)
    {
        //testColor = vec3(0, 0, 1);
        dirColor *= shadowCheck(position, shadowMap2, mtxDirLight2, 0.0025f);
    }
    else
    {
        //testColor = vec3(0, 1, 1);
        dirColor *= shadowCheck(position, shadowMap3, mtxDirLight3, 0.0015f);
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