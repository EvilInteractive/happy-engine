//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#pragma optionNV(fastmath on)
//#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

#include "packing/encode.frag"
#include "shared/perCameraUniformBuffer.frag"
#include "shared/calcNormal.frag"

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passTangent;
in float passDepth;

out vec4 outColor;
out vec4 outSG;
out vec3 outNormalDepth;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specGlossIllMap;

void main()
{
    vec4 color = texture(diffuseMap, passTexCoord);
    
    if (color.a < 0.5f)
        discard;
    
    vec3 normal = texture(normalMap, passTexCoord).rgb * 1;
    vec4 specGlossIll = texture(specGlossIllMap, passTexCoord);
    
    outColor = vec4(color.rgb, specGlossIll.b);

    outSG = vec4(specGlossIll.rg, 0.0f, 1.0f);
    
    outNormalDepth.xy = encodeNormal(calcNormal(passNormal, passTangent, normal));
    outNormalDepth.z = (passDepth - perCameraUniformBuffer.cameraNearFar.x) / (perCameraUniformBuffer.cameraNearFar.y - perCameraUniformBuffer.cameraNearFar.x);
}