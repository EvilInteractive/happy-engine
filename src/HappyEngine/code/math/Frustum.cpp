//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 19/07/2012
#include "HappyPCH.h" 

#include "Frustum.h"

#include "Sphere.h"
#include "AABB.h"

namespace he {

Frustum::Frustum()
{
}

IntersectResult Frustum::intersect( const Sphere& sphere ) const
{
    float distance(0.0f);

    for(uint i(0); i < 6; ++i) 
    {
        distance = dot(m_Plane[i].getNormal(), sphere.getPosition()) - m_Plane[i].getDistance();

        if(distance < -sphere.getRadius())
            return IntersectResult_Outside;

        if(fabs(distance) < sphere.getRadius())
            return IntersectResult_Intersecting;
    }

    return IntersectResult_Inside;
}

IntersectResult Frustum::intersect( const AABB& aabb ) const
{
    int totalPointsInside(0);
    
    for (uint plane(0); plane < 6; ++plane)
    {
        switch (m_Plane[plane].intersect(aabb))
        { 
        case IntersectResult_Outside: 
            return IntersectResult_Outside;
        case IntersectResult_Inside:
            ++totalPointsInside; break;
        case IntersectResult_Intersecting:
            break;
        }
    }

    if (totalPointsInside == 6)
        return IntersectResult_Inside;
    
    return IntersectResult_Intersecting;
}

void Frustum::generateFrustumPoints( std::vector<vec3>& pointBuffer ) const
{
    // 0 : FTL 
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_NearClipPlane], m_Plane[FrustumPlane_TopClipPlane], m_Plane[FrustumPlane_LeftClipPlane]));
    // 1 : FTR  
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_NearClipPlane], m_Plane[FrustumPlane_TopClipPlane], m_Plane[FrustumPlane_RightClipPlane]));    
    // 2 : FBL
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_NearClipPlane], m_Plane[FrustumPlane_BottomClipPlane], m_Plane[FrustumPlane_LeftClipPlane]));      
    // 3 : FBR      
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_NearClipPlane], m_Plane[FrustumPlane_BottomClipPlane], m_Plane[FrustumPlane_RightClipPlane]));
    // 4 : BTL
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_FarClipPlane], m_Plane[FrustumPlane_TopClipPlane], m_Plane[FrustumPlane_LeftClipPlane]));
    // 5 : BTR
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_FarClipPlane], m_Plane[FrustumPlane_TopClipPlane], m_Plane[FrustumPlane_RightClipPlane]));
    // 6 : BBL
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_FarClipPlane], m_Plane[FrustumPlane_BottomClipPlane], m_Plane[FrustumPlane_LeftClipPlane]));
    // 7 : BBR
    pointBuffer.push_back(Plane::getIntersectionPoint(m_Plane[FrustumPlane_FarClipPlane], m_Plane[FrustumPlane_BottomClipPlane], m_Plane[FrustumPlane_RightClipPlane]));
}

} //end namespace
