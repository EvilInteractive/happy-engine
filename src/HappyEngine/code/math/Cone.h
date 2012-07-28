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

#ifndef _HE_Cone_H_
#define _HE_Cone_H_
#pragma once

namespace he {
class Sphere;
class Frustum;

class Cone
{
public:
    Cone();
    Cone(const vec3& position, const vec3& axis, float fov);

    bool intersectTest(const Sphere& sphere) const;

    inline const vec3& getPosition() const { return m_Position; }
    inline const vec3& getAxis() const { return m_Axis; }
    inline float getFov() const { return m_Fov; }

    void setPosition(const vec3& position) { m_Position = position; }
    void setAxis(const vec3& axis) { m_Axis = axis; }
    void setFov(float fov);

private:

    vec3 m_Position;
    vec3 m_Axis;
    float m_Fov;

    // Cach - faster intersect check
    // m_CosSqr   = cos(m_Fov / 2.0f)^2
    // m_SinSqr   = sin(m_Fov / 2.0f)^2
    // m_SinRecip = 1 / sin(m_Fov / 2.0f)
    float m_CosSqr, m_SinSqr, m_SinRecip;

};

} //end namespace

#endif
