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
//Created: 18/12/2011

#ifndef _HE_ICAMERA_H_
#define _HE_ICAMERA_H_
#pragma once

namespace he {
class Bound;
class Sphere;

namespace gfx {

enum ECameraType
{
    eCameraType_Perspective,
    eCameraType_Ortho,
};

class HAPPY_ENTRY ICamera
{
public:
    virtual ~ICamera() {}
    
    // Getters
    virtual const mat44& getView() const = 0;
    virtual const mat44& getProjection() const = 0;
    virtual const mat44& getViewProjection() const = 0;
    virtual float getNearClip() const = 0;
    virtual float getFarClip() const = 0;
    virtual const vec3& getPosition() const = 0;
    virtual const vec3& getLook() const = 0;
    virtual ECameraType getCameraType() = 0;

    // Setters
    virtual void setPosition(const vec3& pos) = 0;
    virtual void setAspectRatio(float aspectRatio) = 0;
    virtual void setFov( const float fov ) = 0;
    virtual void setNearFarPlane(float nearZ, float farZ) = 0;
    virtual void setEyeShift(const float lookShift, const float projShift) = 0;

    virtual void tick(const float dTime) = 0;
    virtual void prepareForRendering() = 0;

    virtual IntersectResult intersect(const Bound& bound) const = 0;
    virtual IntersectResult intersect(const Sphere& bound) const = 0;
};  

} } //end namespace

#endif
