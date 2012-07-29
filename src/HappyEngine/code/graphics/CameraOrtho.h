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
//Created: 23/07/2012

#ifndef _HE_CameraOrtho_H_
#define _HE_CameraOrtho_H_
#pragma once

#include "ICamera.h"
#include "Sphere.h"
#include "Frustum.h"

namespace he {
namespace gfx {
class CameraOrtho : public ICamera
{
public:
    class CameraBound
    {
    public:
        inline const Sphere& getSphere() const { return m_Sphere; }
        inline const Frustum& getFrustum() const { return m_Frustum; }

        void calculate(const CameraOrtho* cam);

    private:
        Sphere m_Sphere;
        Frustum m_Frustum;
    };

    // CONSTRUCTOR - DESTRUCTOR
    CameraOrtho();
    virtual ~CameraOrtho();

    // ICamera
    virtual const mat44& getView() const { return m_View; }
    virtual const mat44& getProjection() const { return m_Projection; }
    virtual const mat44& getViewProjection() const { return m_ViewProjection; }

    virtual float getNearClip() const { return m_NearZ; }
    virtual float getFarClip() const { return m_FarZ; }

    virtual const vec3& getPosition() const { return m_PosWorld; }
    virtual const vec3& getLook() const { return m_LookWorld; }

    virtual IntersectResult intersect(const Bound& bound) const;
    virtual IntersectResult intersect(const Sphere& bound) const;

    // GENERAL
    virtual void tick(float /*dTime*/) {}
    virtual void lookAt(const vec3& pos, const vec3& target, const vec3& up);

    // SETTERS
    virtual void setPosition(const vec3& pos);
    virtual void setLens(float left, float right, float top, float bottom, float zNear = 10, float zFar = 500);

    // GETTERS
    virtual const vec3& getRight() const { return m_RightWorld; }
    virtual const vec3& getUp() const { return m_UpWorld; }

    virtual float getLeftClip() const { return m_Left; }
    virtual float getRightClip() const { return m_Right; }
    virtual float getTopClip() const { return m_Top; }
    virtual float getBottomClip() const { return m_Bottom; }

protected:

    void buildViewMatrix();
    void buildProjectionMatrix();

    // DATAMEMBERS
    mat44 m_View;
    mat44 m_Projection;
    mat44 m_ViewProjection;

    vec3 m_PosWorld;
    vec3 m_RightWorld;
    vec3 m_UpWorld;
    vec3 m_LookWorld;

    float m_Left, m_Right, m_Top, m_Bottom;
    float m_NearZ, m_FarZ;

    CameraBound m_Bound;
    
private:

    //Disable default copy constructor and default assignment operator
    CameraOrtho(const CameraOrtho&);
    CameraOrtho& operator=(const CameraOrtho&);
};

} } //end namespace

#endif
