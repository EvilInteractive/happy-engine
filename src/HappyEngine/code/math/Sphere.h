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

#ifndef _HE_SPHERE_H_
#define _HE_SPHERE_H_
#pragma once

namespace he {
    
class Sphere
{
public:
    Sphere();
    Sphere(const vec3& pos, float radius);
    //default copy and assignment are fine

    inline const vec3& getPosition() const { return m_Position; }
    inline float getRadius() const { return m_Radius; }

    inline void setPosition(const vec3& pos) { m_Position = pos; }
    inline void setRadius(float radius) { m_Radius = radius; }
    
    bool intersectTest(const Sphere& other) const;
    IntersectResult intersect(const Sphere& other) const;

    static Sphere calculateBoundingSphere(const void* pointCloud, uint num, uint stride, uint posOffset);

private:

    vec3 m_Position;
    float m_Radius;
};

} //end namespace

#endif
