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
#pragma optionNV(fastmath on)
//#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

#include "packing/encode.frag"

in vec2 passTexCoord;
in vec3 passNormal;
in vec3 passTangent;
in float passDepth;

out vec4 outColor;
out vec3 outNormalDepth;
out vec4 outSG;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specGlossIllMap;
uniform vec2 camNearFar;

vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)
{
    //NormalMap
    tangent = normalize(tangent);
    normal = normalize(normal);

    vec3 binormal = cross(tangent, normal);

    mat3 assenstelsel = mat3(binormal, tangent, normal);

    vec3 xyz = vec3(rgb.x * 2 - 1, (1-rgb.y) * 2 - 1, rgb.z * 2 - 1);

    return normalize(assenstelsel * xyz);
}

void main()
{
    vec4 color = texture(diffuseMap, passTexCoord);
    
    if (color.a < 0.5f)
        discard;
    
    vec3 normal = texture(normalMap, passTexCoord).rgb * 1;
    vec4 specGlossIll = texture(specGlossIllMap, passTexCoord);
    
    outColor = vec4(color.rgb, specGlossIll.b);

    outNormalDepth.xy = encodeNormal(calcNormal(passNormal, passTangent, normal));
    outNormalDepth.z = (passDepth - camNearFar.x) / (camNearFar.y - camNearFar.x);

    outSG = vec4(specGlossIll.rg, 0.0f, 1.0f);
}