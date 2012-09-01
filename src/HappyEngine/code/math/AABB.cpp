//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 28/09/2011
#include "HappyPCH.h" 

#include "AABB.h"

namespace he {

AABB AABB::calculateBoundingAABB( const void* pointCloud, uint num, uint stride, uint posOffset )
{
    if (num < 2)
        return AABB(vec3(0, 0, 0), vec3(0, 0, 0));
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX), 
         max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    const char* charPointCloud = static_cast<const char*>(pointCloud);
    for(uint i = 0; i < num; ++i)
    {
        const vec3& p(*reinterpret_cast<const vec3*>(charPointCloud + stride * i + posOffset));
        he::minPerComponent(min, p, min);
        he::maxPerComponent(max, p, max);
    }

    return AABB(min, max);
}

void AABB::generateVertices( std::vector<vec3>& outBuffer ) const
{
    vec3 diff(m_BottomBackRight - m_TopFrontLeft);
    outBuffer.push_back(m_TopFrontLeft + vec3(0, 0, 0));
    outBuffer.push_back(m_TopFrontLeft + vec3(diff.x, 0, 0));
    outBuffer.push_back(m_TopFrontLeft + vec3(0, diff.y, 0));
    outBuffer.push_back(m_TopFrontLeft + vec3(diff.x, diff.y, 0));
    outBuffer.push_back(m_TopFrontLeft + vec3(0, 0, diff.z));
    outBuffer.push_back(m_TopFrontLeft + vec3(diff.x, 0, diff.z));
    outBuffer.push_back(m_TopFrontLeft + vec3(0, diff.y, diff.z));
    outBuffer.push_back(m_TopFrontLeft + vec3(diff.x, diff.y, diff.z));
}

void AABB::generateIndices( std::vector<uint>& outBuffer, uint offset ) const
{
    // Front
    outBuffer.push_back(offset + 0); outBuffer.push_back(offset + 1);
    outBuffer.push_back(offset + 0); outBuffer.push_back(offset + 2);
    outBuffer.push_back(offset + 1); outBuffer.push_back(offset + 3);
    outBuffer.push_back(offset + 2); outBuffer.push_back(offset + 3);

    // Back
    outBuffer.push_back(offset + 4); outBuffer.push_back(offset + 5);
    outBuffer.push_back(offset + 4); outBuffer.push_back(offset + 6);
    outBuffer.push_back(offset + 5); outBuffer.push_back(offset + 7);
    outBuffer.push_back(offset + 6); outBuffer.push_back(offset + 7);

    // Connection
    outBuffer.push_back(offset + 0); outBuffer.push_back(offset + 4);
    outBuffer.push_back(offset + 1); outBuffer.push_back(offset + 5);
    outBuffer.push_back(offset + 2); outBuffer.push_back(offset + 6);
    outBuffer.push_back(offset + 3); outBuffer.push_back(offset + 7);
}

} //end namespace