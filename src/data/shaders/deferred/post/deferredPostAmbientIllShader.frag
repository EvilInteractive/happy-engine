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
};

uniform AmbientLight ambLight;
uniform DirectionalLight dirLight;

layout(shared) uniform SharedBuffer
{
    vec4 projParams;
};

uniform sampler2D colorIllMap;
uniform sampler2D normalDepthMap;
#if SPECULAR
uniform sampler2D sgMap;
#endif

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
    vec3 diffuseLight = dotLightNormal * dirLight.color.rgb * dirLight.color.a;
    vec3 ambientLight = ambLight.color.a * ambLight.color.rgb * ambHalfLambert; 

    //Specular
    vec3 spec = vec3(0, 0, 0);
#if SPECULAR
	vec3 vCamDir = normalize(-position);
	spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r) * 5.0f * dirLight.color.a * dirLight.color.rgb;
#endif

    //Albedo
    vec4 color = texture(colorIllMap, texCoord);
     
    //Out         
    outColor = vec4(((diffuseLight + spec) + ambientLight + vec3(color.a, color.a, color.a) * 10) * color.rgb
                        , 1.0f);		
    //outColor *= vec4(cascade, 1);
}
