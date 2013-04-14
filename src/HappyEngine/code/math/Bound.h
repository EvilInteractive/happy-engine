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

#ifndef _HE_Bound_H_
#define _HE_Bound_H_
#pragma once

#include "AABB.h"
#include "Sphere.h"

namespace he {

class Bound
{
public:
    Bound(): m_AABB(), m_Sphere() {}
    explicit Bound(const AABB& aabb): m_AABB(aabb),
        m_Sphere(
        (aabb.getTopFrontLeft() + aabb.getBottomBackRight()) / 2.0f,
        length(aabb.getBottomBackRight() - aabb.getTopFrontLeft()) / 2.0f)
    {
    }

    void fromAABB(const AABB& aabb)
    {
        m_AABB = aabb;
        m_Sphere.setPosition(
            (aabb.getTopFrontLeft() + aabb.getBottomBackRight()) / 2.0f);
        m_Sphere.setRadius(
            length(aabb.getBottomBackRight() - aabb.getTopFrontLeft()) / 2.0f);
    }

    inline const AABB& getAABB() const { return m_AABB; }
    inline const Sphere& getSphere() const { return m_Sphere; }

    IntersectResult intersect(const Bound& other) const
    {
        if (m_Sphere.intersectTest(other.m_Sphere))
        {
            return m_AABB.intersect(other.m_AABB);
        }
        return IntersectResult_Outside;
    }
    bool intersectTest(const Ray& ray) const
    {
        return m_Sphere.intersectTest(ray);
    }
    bool isOtherInside(const Bound& other) const
    {
        if (m_Sphere.intersectTest(other.m_Sphere))
        {
            return m_AABB.isOtherInside(other.m_AABB);
        }
        return false;
    }

private:

    AABB m_AABB;
    Sphere m_Sphere;
};

} //end namespace

#endif
