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
#include "shared/perCameraUniformBuffer.frag"

out vec4 outColor;
out vec3 outNormalDepth;

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passTangent;
in vec3 passPosition;

uniform sampler2D diffuseMap;
#if NORMALMAP
uniform sampler2D normalMap;
#endif
uniform sampler2D specGlossIllMap;

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
    vec3 position = passPosition;
    vec3 lightDir = normalize(dirLight.direction);      
    vec3 normal = passNormal;
#if NORMALMAP
    normal = calcNormal(passNormal, passTangent, texture(normalMap, passTexCoord).rgb);
#endif
    
    //Lambert
    float dotLightNormal = clamp(dot(lightDir, normal), 0.0f, 1.0f);   
	float ambHalfLambert = clamp(dot(-lightDir, normal), 0.0f, 1.0f) * 0.5f + 0.5f;
    
    //Light
    vec3 diffuseLight = dotLightNormal * dirLight.color.rgb * dirLight.color.a;
    vec3 ambientLight = ambLight.color.a * ambLight.color.rgb * ambHalfLambert; 

    //Specular
    vec3 spec = vec3(0, 0, 0);
#if SPECULAR
	vec3 vCamDir = normalize(-position);
	spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r) * 5.0f * dirLight.color.a * dirLight.color.rgb;
#endif
             
    //Albedo
    vec4 color = texture(diffuseMap, passTexCoord);
    
    // Depth
    float normalizedDepth = (position.z - perCameraUniformBuffer.cameraNearFar.x) / (perCameraUniformBuffer.cameraNearFar.y - perCameraUniformBuffer.cameraNearFar.x);
     
    //Out         
    outColor = vec4(((diffuseLight + spec) + ambientLight + vec3(color.a, color.a, color.a) * 10) * color.rgb
                        , 1.0f);
    outNormalDepth = vec3(encodeNormal(normal), normalizedDepth);
}