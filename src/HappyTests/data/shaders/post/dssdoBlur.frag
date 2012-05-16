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
//Created: 14/10/2011
//Name change: HDRtoLDR -> postEffects : 18/12/2011

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

float4 dssdo_blur(float2 tex, float2 dir) : COLOR
{
	float weights[9] =
	{
		0.013519569015984728,
		0.047662179108871855,
		0.11723004402070096,
		0.20116755999375591,
		0.240841295721373,
		0.20116755999375591,
		0.11723004402070096,
		0.047662179108871855,
		0.013519569015984728
	};

	float indices[9] = {-4, -3, -2, -1, 0, +1, +2, +3, +4};

	float2 step = dir/g_resolution.xy;

	float3 normal[9];

	normal[0] = tex2D(smp_normal, tex + indices[0]*step).xyz;
	normal[1] = tex2D(smp_normal, tex + indices[1]*step).xyz;
	normal[2] = tex2D(smp_normal, tex + indices[2]*step).xyz;
	normal[3] = tex2D(smp_normal, tex + indices[3]*step).xyz;
	normal[4] = tex2D(smp_normal, tex + indices[4]*step).xyz;
	normal[5] = tex2D(smp_normal, tex + indices[5]*step).xyz;
	normal[6] = tex2D(smp_normal, tex + indices[6]*step).xyz;
	normal[7] = tex2D(smp_normal, tex + indices[7]*step).xyz;
	normal[8] = tex2D(smp_normal, tex + indices[8]*step).xyz;

	float total_weight = 1.0;
	float discard_threshold = 0.85;

	int i;

	for( i=0; i<9; ++i )
	{
		if( dot(normal[i], normal[4]) < discard_threshold )
		{
			total_weight -= weights[i];
			weights[i] = 0;
		}
	}

	//

	float4 res = 0;

	for( i=0; i<9; ++i )
	{
		res += tex2D(smp_occlusion, tex + indices[i]*step) * weights[i];
	}

	res /= total_weight;

	return res;
}