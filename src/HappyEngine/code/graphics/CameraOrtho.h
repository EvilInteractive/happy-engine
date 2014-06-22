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
    const mat44& getView() const { return m_View; } // override
    const mat44& getProjection() const { return m_Projection; } // override
    const mat44& getViewProjection() const { return m_ViewProjection; } // override
    float getNearClip() const { return m_NearZ; } // override
    float getFarClip() const { return m_FarZ; } // override
    const vec3& getPosition() const { return m_PosWorld; } // override
    const vec3& getLook() const { return m_LookWorld; } // override
    ECameraType getCameraType() { return eCameraType_Ortho; } // override

    IntersectResult intersect(const Bound& bound) const; // override
    IntersectResult intersect(const Sphere& bound) const; // override

    // GENERAL
    virtual void prepareForRendering(); // override
    virtual void tick(const float /*dTime*/) {} // override
    void lookAt(const vec3& pos, const vec3& target, const vec3& up);

    // SETTERS
    void setPosition(const vec3& pos); // override
    void setAspectRatio(float aspectRatio);  // override
    void setFov( const float /*fov*/ ) { LOG(LogType_ProgrammerAssert, "Ortho camera does not support fov"); } // override
    void setNearFarPlane(float nearZ, float farZ); // override
    void setEyeShift(const float /*lookShift*/, const float /*projShift*/) { LOG(LogType_ProgrammerAssert, "Ortho camera does not support eyeshift"); } // override
    void setLens(float left, float right, float top, float bottom, float zNear = 1.0f, float zFar = 500.0f);

    // GETTERS
    const vec3& getRight() const { return m_RightWorld; }
    const vec3& getUp() const { return m_UpWorld; }

    float getLeftClip() const { return m_Left; }
    float getRightClip() const { return m_Right; }
    float getTopClip() const { return m_Top; }
    float getBottomClip() const { return m_Bottom; }

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
    bool m_RegenViewMatrix;
    bool m_RegenProjMatrix;
    
private:

    //Disable default copy constructor and default assignment operator
    CameraOrtho(const CameraOrtho&);
    CameraOrtho& operator=(const CameraOrtho&);

};

} } //end namespace

#endif
