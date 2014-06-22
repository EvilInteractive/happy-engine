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
//Author:  Bastian Damman
//Created: 19/05/2012

#ifndef _HE_Ray_H_
#define _HE_Ray_H_
#pragma once

namespace he {
namespace px {
    class PhysicsActor;
}
namespace gfx {
    class ICamera;
    class View;
}

struct RayCastResult
{
    bool hit;
    vec3 hitPosition;
    vec3 hitNormal;
    float hitDistance;
    px::PhysicsActor* actorHit;
};

class HAPPY_ENTRY Ray
{
public:
    Ray(const vec3& position, const vec3& direction, float maxDist = FLT_MAX);
    Ray(const gfx::View* view, const gfx::ICamera* camera, const vec2& mousePos, float maxDist = FLT_MAX);
    ~Ray();

    inline const vec3& getOrigin() const { return m_Origin; }
    inline const vec3& getDirection() const { return m_Direction; }
    inline float getMaxDistance() const { return m_MaxDistance; }

    Ray transform(const mat44& world) const;

private:
    vec3 m_Origin;
    vec3 m_Direction;
    float m_MaxDistance;
};

} //end namespace

#endif
