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

struct ToneMapData
{
	float shoulderStrength;
	float linearStrength;
	float linearAngle;
	float toeStrength;
	float toeNumerator;
	float toeDenominator;
	float exposureBias;
};

float getWhite(in sampler2D lumMap, in float min, in float max)
{
    return clamp(textureLod(lumMap, vec2(0.5f, 0.5f), 0).r, min, max);
}
vec3 tonemapFunc(in vec3 x, in ToneMapData data)
{
	return ((x * (data.shoulderStrength * x + data.linearAngle * data.linearStrength) + data.toeStrength * data.toeNumerator) / 
			(x * (data.shoulderStrength * x + data.linearStrength) + data.toeStrength * data.toeDenominator)) - data.toeNumerator / data.toeDenominator;
}
vec3 tonemap(in vec3 hdr, in vec3 whitePoint)
{
	vec3 ldr = hdr;
	
	ToneMapData data;
	data.shoulderStrength = 0.22f;
	data.linearStrength = 0.30f;
	data.linearAngle = 0.10f;
	data.toeStrength = 0.01f;
	data.toeNumerator = 0.30f;
	data.toeDenominator = 11.2f;
	data.exposureBias = 1.0f;
	
	ldr = tonemapFunc(ldr*data.exposureBias, data) / whitePoint;
	
	return ldr;
}
