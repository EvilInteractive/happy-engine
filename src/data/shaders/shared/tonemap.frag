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

#ifndef _FRAG_TONEMAP
#define _FRAG_TONEMAP

#include "shared/perSceneUniformBuffer.frag"

float getWhite(in sampler2D lumMap, in float min, in float max)
{
    return clamp(textureLod(lumMap, vec2(0.5f, 0.5f), 0).r, min, max);
}
vec3 tonemapFunc(in vec3 x)
{
	return ((x * (perSceneUniformBuffer.shoulderStrength * x + perSceneUniformBuffer.linearAngle * perSceneUniformBuffer.linearStrength) + 
            perSceneUniformBuffer.toeStrength * perSceneUniformBuffer.toeNumerator) / 
			(x * (perSceneUniformBuffer.shoulderStrength * x + perSceneUniformBuffer.linearStrength) + 
            perSceneUniformBuffer.toeStrength * perSceneUniformBuffer.toeDenominator)) - 
            perSceneUniformBuffer.toeNumerator / perSceneUniformBuffer.toeDenominator;
}
vec3 tonemap(in vec3 hdr, in vec3 whitePoint)
{
	vec3 ldr = hdr;
		
	ldr = tonemapFunc(ldr*perSceneUniformBuffer.exposureBias) / tonemapFunc(whitePoint);
	
	return ldr;
}

#endif // _FRAG_TONEMAP
