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

vec3 getPosition(in float p_depth, in vec2 p_ndc, in vec4 p_projParams)
{ 
    float depth = p_depth * 2.0f - 1.0f;
    float linDepth = p_projParams.w / (depth - p_projParams.z);

    return vec3((p_ndc * linDepth) / p_projParams.xy, linDepth);
}


//source: http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
//vec3 decodeNormal(in vec2 packedNormal)
//{
    //vec4 nn = vec4(packedNormal * 2 + vec2(-1, -1), 1, -1);
    //float l = dot(nn.xyz, -nn.xyw);
    //nn.z = l;
    //nn.xy *= sqrt(l);
    //return nn.xyz * 2 + vec3(0, 0, -1);
//}
//
vec3 decodeNormal(in vec2 packedNormal)
{
    vec2 fenc = packedNormal * 4.0f - 2.0f;
    float f = dot(fenc, fenc);
    float g = sqrt(f * -0.25f + 1.0f);
    return vec3(fenc*g, f * 0.5f - 1.0f);
}
