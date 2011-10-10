//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

namespace he {
namespace gfx {

class Camera
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Camera(int viewportWidth, int viewportHeight);
    virtual ~Camera();

	// GENERAL
	virtual void resize(int viewportWidth, int viewportHeight);
	virtual void lookAt(const vec3& pos, const vec3& target, const vec3& up);

	// SETTERS
	virtual void setPosition(const vec3& pos);
	virtual void setLens(float aspectRatio = (16.0f/9.0f), float fov = piOverFour, float nearZ = 10.0f, float farZ = 1000.0f);
	virtual void setActive(bool active);

	// GETTERS
	virtual const mat44& getView() const
	{ return m_matView; }
	virtual const mat44& getProjection() const
	{ return m_matProjection; }
	virtual const mat44& getViewProjection() const
	{ return m_matViewProjection; }

	virtual const vec3& getPosition() const
	{ return m_vPosWorld; }
	virtual const vec3& getRight() const
	{ return m_vRightWorld; }
	virtual const vec3& getUp() const
	{ return m_vUpWorld; }
	virtual const vec3& getLook() const
	{ return m_vLookWorld; }

    virtual float getNearClip() const
    { return m_NearZ; }
    virtual float getFarClip() const
    { return m_FarZ; }
    virtual float getFov() const
    { return m_FOV; }
    virtual float getAspectRatio() const
    { return m_AspectRatio; }

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

	float m_FOV;
	float m_AspectRatio;
	float m_NearZ, m_FarZ;

	bool m_bIsActive;

private:

    //Disable default copy constructor and default assignment operator
    Camera(const Camera&);
    Camera& operator=(const Camera&);
};

} } //end namespace

#endif
