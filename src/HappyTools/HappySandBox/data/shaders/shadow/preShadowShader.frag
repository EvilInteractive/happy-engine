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

in vec4 passPos;

uniform vec2 inLightNearFar;

out vec2 outShadow;
  
/*vec4 DistributePrecision(vec2 moments)
{
    float factorInv = 1.0f / 256.0f;
    
    // Split precision
    vec2 intPart;
    vec2 fracPart = modf(moments * 256.0f, intPart);
    // Compose outputs to make reconstruction cheap.
    return vec4(intPart * factorInv, fracPart);
}*/

vec2 ComputeMoments(in float depth)
{
    vec2 moments;
    moments.x = depth;
    
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    
    moments.y = depth*depth + 0.25f * (dx*dx + dy*dy);
    return moments;
}

void main()
{
    float depth = (passPos.z - inLightNearFar.x) / inLightNearFar.y;
    //outShadow = DistributePrecision(ComputeMoments(depth));
    outShadow = ComputeMoments(depth);
}