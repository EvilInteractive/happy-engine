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
//Created: 2013/03/02

#ifndef _HE_PickResult_H_
#define _HE_PickResult_H_
#pragma once

namespace he {
namespace ge {

class Pickable;

class PickResult
{
public:
    PickResult(): m_HitObject(nullptr), m_HitPosition(0, 0, 0), m_HitNormal(0, 1, 0), m_HitDistance(FLT_MAX) {}

    inline bool isHit() const { return m_HitObject != nullptr; }
    inline const vec3& getHitPosition() const { return m_HitPosition; }
    inline const vec3& getHitNormal() const { return m_HitNormal; }
    inline float getHitDistance() const { return m_HitDistance; }
    inline Pickable* getObject() const { return m_HitObject; }

    void reset() { m_HitObject = nullptr; m_HitDistance = FLT_MAX; }

    void setHit(Pickable* const object, const vec3& position, const vec3& normal, const float distance)
    {
        m_HitObject = object;
        m_HitPosition = position;
        m_HitNormal = normal;
        m_HitDistance = distance;
    }

private:
    Pickable* m_HitObject;
    vec3 m_HitPosition;
    vec3 m_HitNormal;
    float m_HitDistance;
};

} } //end namespace

#endif
