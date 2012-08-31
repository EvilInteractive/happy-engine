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

#ifndef _HE_Frustum_H_
#define _HE_Frustum_H_
#pragma once

#include "Plane.h"

namespace he {
class Sphere;
class AABB;

enum FrustumPlane
{
    FrustumPlane_LeftClipPlane,
    FrustumPlane_RightClipPlane,
    FrustumPlane_TopClipPlane,
    FrustumPlane_BottomClipPlane,
    FrustumPlane_NearClipPlane,
    FrustumPlane_FarClipPlane
};

class Frustum
{
public:
    Frustum();

    IntersectResult intersect(const Sphere& sphere) const;
    IntersectResult intersect(const AABB& aabb) const;

    Plane& getPlaneForEdit(FrustumPlane plane) { return m_Plane[plane]; }

    // Defined as:
    // FTL = Front Top Left, BBR = Bottom Back Right
    // All seen from position of camera
    // 0 : FTL      // 4: BTL
    // 1 : FTR      // 5: BTR
    // 2 : FBL      // 6: BBL
    // 3 : FBR      // 7: BBR
    void generateFrustumPoints(std::vector<vec3>& pointBuffer) const;

private:
    Plane m_Plane[6];

};

} //end namespace

#endif
