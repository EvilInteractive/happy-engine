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
//    along with HappyEngine.  If not, see <http://www.gnu.org/8licenses/>.
//
//Author:  Bastian Damman
//Created: 06/05/2012

#version 150 core
#pragma optionNV(fastmath on)
#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

#include "packing/decode.frag"

noperspective in vec2 texCoord;

out vec4 outColor;

uniform float sampleRadius;
uniform float maxDistance;
uniform vec2 screenSize;
uniform vec4 projParams;
//uniform vec3 camPos;

uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D noiseMap;

vec4 calculateDssdo()
{
    const int num_points = 32;
    const vec3 points[32] = vec3[32]
    (
        vec3(-0.134f,  0.044f, -0.825f),
        vec3( 0.045f, -0.431f, -0.529f),
        vec3(-0.537f,  0.195f, -0.371f),
        vec3( 0.525f, -0.397f,  0.713f),
        vec3( 0.895f,  0.302f,  0.139f),
        vec3(-0.613f, -0.408f, -0.141f),
        vec3( 0.307f,  0.822f,  0.169f),
        vec3(-0.819f,  0.037f, -0.388f),
        vec3( 0.376f,  0.009f,  0.193f),
        vec3(-0.006f, -0.103f, -0.035f),
        vec3( 0.098f,  0.393f,  0.019f),
        vec3( 0.542f, -0.218f, -0.593f),
        vec3( 0.526f, -0.183f,  0.424f),
        vec3(-0.529f, -0.178f,  0.684f),
        vec3 (0.066f, -0.657f, -0.570f),
        vec3(-0.214f,  0.288f,  0.188f),
        vec3(-0.689f, -0.222f, -0.192f),
        vec3(-0.008f, -0.212f, -0.721f),
        vec3( 0.053f, -0.863f,  0.054f),
        vec3( 0.639f, -0.558f,  0.289f),
        vec3(-0.255f,  0.958f,  0.099f),
        vec3(-0.488f,  0.473f, -0.381f),
        vec3(-0.592f, -0.332f,  0.137f),
        vec3( 0.080f,  0.756f, -0.494f),
        vec3(-0.638f,  0.319f,  0.686f),
        vec3(-0.663f,  0.230f, -0.634f),
        vec3( 0.235f, -0.547f,  0.664f),
        vec3( 0.164f, -0.710f,  0.086f),
        vec3(-0.009f,  0.493f, -0.038f),
        vec3(-0.322f,  0.147f, -0.105f),
        vec3(-0.554f, -0.725f,  0.289f),
        vec3( 0.534f,  0.157f, -0.250f)
    );

    vec2 noiseTextureSize = vec2(4, 4);
    vec2 nDc = texCoord * 2.0f - 1.0f;
    float depth = textureLod(depthMap, texCoord, 0).r;
    vec3 position = getPosition(depth, nDc, projParams);
    
    float radius = sampleRadius / position.z;
    float maxDistanceInv = 1.0f / maxDistance;
    float attenuationAngleThreshold = 0.1f;

    vec3 noise = texture(noiseMap, texCoord * screenSize.xy / noiseTextureSize).xyz * 2.0f - 1.0f;
    
    vec3 normal = decodeNormal(texture(normalMap, texCoord).rg);

    vec4 occlusionSh2 = vec4(0, 0, 0, 0);

    const float fudgeFactorL0 = 2.0;
    const float fudgeFactorL1 = 10.0;

    const float sh2WeightL0 = fudgeFactorL0 * 0.28209f;        // 0.5f * sqrt(1.0 / pi);
    const vec3  sh2WeightL1 = vec3(fudgeFactorL1 * 0.48860f);  // 0.5f * sqrt(3.0 / pi);

    const vec4 sh2Weight = vec4(sh2WeightL1, sh2WeightL0) / num_points;

    for( int i = 0; i < num_points; ++i )
    {
        vec2 texOffset = reflect(points[i].xy, noise.xy) * radius;
        vec3 samplePos = getPosition(textureLod(depthMap, texCoord + texOffset, 0).r, nDc, projParams);

        vec3 dirToSample = samplePos - position;
        float distToSample = length(dirToSample);
        dirToSample /= distToSample;

        float attenuation = clamp(distToSample * maxDistanceInv, 0.0f, 1.0f);
        float dp = dot(normal, dirToSample);

        attenuation = attenuation * attenuation * step(attenuationAngleThreshold, dp);

        occlusionSh2 += attenuation * sh2Weight * vec4(dirToSample, 1);
    }

    return occlusionSh2 * 0.5f + 0.5f;
}

void main()
{
    outColor = calculateDssdo();
}
