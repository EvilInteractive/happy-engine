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
//Author:  Sebastiaan Sprengers
//Created: 31/08/2011

#ifndef _HE_CAMERA_H_
#define _HE_CAMERA_H_
#pragma once

#include "vec3.h"
#include "mat44.h"
#include "MathConstants.h"

#include "ICamera.h"

namespace he {
namespace gfx {

class Camera : public ICamera
{
public:

    // CONSTRUCTOR - DESTRUCTOR
    Camera(int viewportWidth, int viewportHeight);
    virtual ~Camera();

    // GENERAL
    virtual void tick(float /*dTime*/) {}
    virtual void resize(int viewportWidth, int viewportHeight);
    virtual void lookAt(const vec3& pos, const vec3& target, const vec3& up);

    // SETTERS
    virtual void setPosition(const vec3& pos);
    virtual void setLens(float aspectRatio = (16.0f/9.0f), float fov = piOverFour, float nearZ = 10.0f, float farZ = 1000.0f);

    virtual void setFxPositionOffset(const vec3& offset);

    // GETTERS
    virtual const mat44& getView() const;
    virtual const mat44& getProjection() const;
    virtual const mat44& getViewProjection() const;

    virtual vec3 getPosition() const;
    virtual const vec3& getRight() const;
    virtual const vec3& getUp() const;
    virtual const vec3& getLook() const;

    virtual float getNearClip() const;
    virtual float getFarClip() const;
    virtual float getFov() const;
    virtual float getAspectRatio() const;

protected:

    void buildViewMatrix();
    void buildProjectionMatrix();

    // DATAMEMBERS
    mat44 m_matView;
    mat44 m_matProjection;
    mat44 m_matViewProjection;

    vec3 m_vPosWorld;
    vec3 m_vRightWorld;
    vec3 m_vUpWorld;
    vec3 m_vLookWorld;

    vec3 m_vFxPosOffset;

    float m_FOV;
    float m_AspectRatio;
    float m_NearZ, m_FarZ;
    
private:

    //Disable default copy constructor and default assignment operator
    Camera(const Camera&);
    Camera& operator=(const Camera&);
};

} } //end namespace

#endif
