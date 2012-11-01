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
//Modified: 08/10/2011: optimizing rendertargets

#version 150 core

#include "packing/decode.frag"

in vec3 passPos;
out vec4 outColor;

struct PointLight
{
    vec3 position;
    float multiplier;
    vec3 color;
    float beginAttenuation;
    float endAttenuation;
};

layout(shared) uniform SharedBuffer
{
    vec4 projParams;
};


uniform PointLight light;


uniform sampler2D colorIllMap;
uniform sampler2D normalDepthMap;
#if SPECULAR
uniform sampler2D sgMap;
#endif


void main()
{
    vec2 ndc = passPos.xy / passPos.z;
    vec2 texCoord = ndc * 0.5f + 0.5f;
    
    vec3 normalDepth = texture(normalDepthMap, texCoord).xyz;
    vec3 position = getPosition( normalDepth.z, ndc, projParams );

    vec3 lightDir = light.position - position;
    float lightDist = length(lightDir);

    if (lightDist > light.endAttenuation) //pixel is too far from light
        discard;

    // Normalize calculated light dir
    lightDir /= lightDist;
    
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
          light.color * light.multiplier * attenuationValue, 1.0f);
}
