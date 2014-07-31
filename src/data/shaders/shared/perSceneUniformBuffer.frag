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

#if _FRAG_PERSCENEUNIFORMBUFFER
#define _FRAG_PERSCENEUNIFORMBUFFER

struct PerSceneUniformBuffer
{
    // HDR
	float shoulderStrength;
	float linearStrength;
	float linearAngle;
	float toeStrength;
	float toeNumerator;
	float toeDenominator;
	float exposureBias;
	float __pad_HDR;
    
    // Light
    vec4 ambLightColor;
    vec4 dirLightColor;
    vec3 dirLightDirection;
    float __pad_Light
};

layout(std140) uniform PerSceneUniformBuffer
{
	PerFrameUniformBuffer perSceneUniformBuffer;
};

#endif // _FRAG_PERSCENEUNIFORMBUFFER
