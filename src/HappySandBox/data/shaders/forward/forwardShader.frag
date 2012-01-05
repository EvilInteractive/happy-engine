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
//Author: Bastian Damman

#version 150 core

#include "packing/encode.frag"

out vec4 outColor;
out vec2 outNormal;

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passTangent;
in vec3 passPosition;


uniform sampler2D diffuseMap;
#if NORMALMAP
uniform sampler2D normalMap;
#endif
uniform sampler2D specGlossIllMap;
uniform sampler2D colorRamp;

struct AmbientLight
{
    vec4 color;
};

struct DirectionalLight
{
    vec4 color;
    vec3 direction;
};

uniform DirectionalLight dirLight;
uniform AmbientLight ambLight;

#if SHADOWS
uniform mat4 mtxDirLight0;
uniform mat4 mtxDirLight1;
uniform mat4 mtxDirLight2;
uniform mat4 mtxDirLight3;

uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
#endif

float shadowCheck(in vec3 position, in sampler2D sampler, in mat4 lightMatrix)
{
    vec4 coord = lightMatrix * vec4(position, 1.0f);
    coord.xyz /= coord.w;
    if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1)
        return 1.0f;

    //NDC -> texturespace
    coord.x = coord.x * 0.5f + 0.5f;
    coord.y = coord.y * 0.5f + 0.5f;

    vec2 map = texture(sampler, coord.xy).rg;

    float fAvgZ = map.x;
    float fAvgZ2 = map.y;

    if (coord.z <= fAvgZ) return 1.0f;
    if (coord.z >= 1.0f) return 0.0f;

    float variance = fAvgZ2 - (fAvgZ * fAvgZ);
    variance = min(max(variance, 0.0f) + 0.001f, 1.0f);

    float mean = fAvgZ;
    float d = coord.z - mean;
    
    return pow(variance / (variance + d*d), 50);
}

vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)
{
    //NormalMap
    tangent = normalize(tangent);
    normal = normalize(normal);

    vec3 binormal = normalize(cross(tangent, normal));

    mat3 assenstelsel = mat3(binormal, tangent, normal);

    vec3 xyz = vec3(rgb.x * 2 - 1, (1-rgb.y) * 2 - 1, rgb.z * 2 - 1);

    return normalize(assenstelsel * xyz);
}

void main()
{
    vec2 ndc = passTexCoord * 2.0f - 1.0f;
        
    vec3 position = passPosition;

    vec3 lightDir = normalize(dirLight.direction);
          
    vec3 normal = passNormal;
#if NORMALMAP
    normal = calcNormal(passNormal, passTangent, texture(normalMap, passTexCoord).rgb);
#endif

    //Lambert
    float dotLightNormal = clamp(dot(lightDir, normal), 0.0f, 1.0f);

    //Ramp
    vec3 lambertRamp = texture(colorRamp, vec2(dotLightNormal, 0.0f)).rgb;

    //HalfLambert
    float halfLambert = dotLightNormal * 0.5f + 0.5f;
    halfLambert *= halfLambert;

    //Light
    vec3 diffuseLight = pow(lambertRamp * halfLambert, vec3(0.3f, 0.3f, 0.3f)) * dirLight.color.rgb * dirLight.color.a;
    vec3 ambientLight = ambLight.color.a * ambLight.color.rgb;
    
    //Shadow
    float shadow = 1;
#if SHADOWS
    if (position.z < 30)
        shadow *= shadowCheck(position, shadowMap0, mtxDirLight0);
    if (position.z > 20 && position.z < 80)
        shadow *= shadowCheck(position, shadowMap1, mtxDirLight1);
    if (position.z > 70 && position.z < 155)
        shadow *= shadowCheck(position, shadowMap2, mtxDirLight2);
    if (position.z > 145)
        shadow *= shadowCheck(position, shadowMap3, mtxDirLight3);
#endif

    //Specular
    vec3 spec = vec3(0.0f, 0.0f, 0.0f);
    vec4 sgi = texture(specGlossIllMap, passTexCoord);
#if SPECULAR
    if (shadow > 0.001f && halfLambert > 0.001f)
    {	
        vec3 vCamDir = normalize(-position);
        spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sgi.g * 100.0f) * sgi.r) * 10.0f * dirLight.color.rgb;
    }
#endif

    //Albedo
    vec4 color = texture(diffuseMap, passTexCoord);
     
    //Out         
    outNormal = encodeNormal(normal);
    outColor = vec4(((diffuseLight + spec) * shadow + ambientLight + vec3(sgi.b, sgi.b, sgi.b) * 10) * color.rgb
                        , 1.0f);
}