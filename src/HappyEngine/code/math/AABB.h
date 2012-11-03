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

#ifndef _HE_AABB_H_
#define _HE_AABB_H_
#pragma once

namespace he {
    
class AABB
{
public:
    AABB(): m_TopFrontLeft(0, 0, 0), m_BottomBackRight(0, 0, 0) {}
    AABB(const AABB& other): m_TopFrontLeft(other.m_TopFrontLeft), m_BottomBackRight(other.m_BottomBackRight) {}
    AABB(const vec3& topFrontLeft, const vec3& bottomBackRight): m_TopFrontLeft(topFrontLeft), m_BottomBackRight(bottomBackRight) {}

    inline const vec3& getTopFrontLeft()     const { return m_TopFrontLeft; }
    inline const vec3& getBottomBackRight()  const { return m_BottomBackRight; }

    inline void setTopFrontLeft(const vec3& tfl)     { m_TopFrontLeft = tfl; }
    inline void setBottomBackRight(const vec3& bbr)  { m_BottomBackRight = bbr; }
    
    static AABB merge(const AABB& a, const AABB& b)
    {
        AABB ret(a);
        ret.merge(b);
        return ret;
    }
    void merge(const AABB& other)
    {
        he::minPerComponent(m_TopFrontLeft, other.m_TopFrontLeft, m_TopFrontLeft);
        he::maxPerComponent(m_BottomBackRight, other.m_BottomBackRight, m_BottomBackRight);
    }

    IntersectResult intersect(const AABB& other) const
    {
        if (isOtherInside(other))
            return IntersectResult_Inside;
        else 
        {
            if (m_TopFrontLeft.x < other.m_BottomBackRight.x &&
                m_TopFrontLeft.y < other.m_BottomBackRight.y &&
                m_TopFrontLeft.z < other.m_BottomBackRight.z &&

                m_BottomBackRight.x > other.m_TopFrontLeft.x &&
                m_BottomBackRight.y > other.m_TopFrontLeft.y &&
                m_BottomBackRight.z > other.m_TopFrontLeft.z)
                return IntersectResult_Intersecting;
            else
                return IntersectResult_Outside;
        }
    }

    bool isOtherInside(const AABB& other) const
    {
        return (m_TopFrontLeft.x <= other.m_TopFrontLeft.x &&
                m_TopFrontLeft.y <= other.m_TopFrontLeft.y &&
                m_TopFrontLeft.z <= other.m_TopFrontLeft.z &&
                
                m_BottomBackRight.x >= other.m_BottomBackRight.x &&
                m_BottomBackRight.y >= other.m_BottomBackRight.y &&
                m_BottomBackRight.z >= other.m_BottomBackRight.z);
    }

    bool isOtherInside(const vec3& other) const
    {
        return (m_TopFrontLeft.x <= other.x &&
                m_TopFrontLeft.y <= other.y &&
                m_TopFrontLeft.z <= other.z &&

                m_BottomBackRight.x >= other.x &&
                m_BottomBackRight.y >= other.y &&
                m_BottomBackRight.z >= other.z);
    }

    static AABB calculateBoundingAABB(const void* pointCloud, uint32 num, uint32 stride, uint32 posOffset);

    void generateVertices(std::vector<vec3>& outBuffer) const;
    void generateIndices(std::vector<uint32>& outBuffer, uint32 offset) const;

private:
    vec3 m_TopFrontLeft; 
    vec3 m_BottomBackRight;
};

} //end namespace

#endif
