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

#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#include "Vector3.h"
#include "Matrix.h"
#include "MathConstants.h"

namespace happyengine {
namespace graphics {

class Camera
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Camera(int viewportWidth, int viewportHeight);
    virtual ~Camera();

	// GENERAL
	virtual void Resize(int viewportWidth, int viewportHeight);
	virtual void LookAt(const math::Vector3 &pos, const math::Vector3 &target, const math::Vector3 &up);

	// SETTERS
	virtual void SetPosition(const math::Vector3 &pos);
	virtual void SetLens(float aspectRatio = (16.0f/9.0f), float fov = math::piOverFour, float nearZ = 10.0f, float farZ = 1000.0f);
	virtual void SetActive(bool active);

	// GETTERS
	virtual math::Matrix GetView() const
	{ return m_matView; }
	virtual math::Matrix GetProjection() const
	{ return m_matProjection; }
	virtual math::Matrix GetViewProjection() const
	{ return m_matViewProjection; }

	virtual math::Vector3 GetPosition() const
	{ return m_vPosWorld; }
	virtual math::Vector3 GetRight() const
	{ return m_vRightWorld; }
	virtual math::Vector3 GetUp() const
	{ return m_vUpWorld; }
	virtual math::Vector3 GetLook() const
	{ return m_vLookWorld; }

protected:

	void BuildViewMatrix();
	void BuildProjectionMatrix();

	// DATAMEMBERS
	math::Matrix m_matView;
	math::Matrix m_matProjection;
	math::Matrix m_matViewProjection;

	math::Vector3 m_vPosWorld;
	math::Vector3 m_vRightWorld;
	math::Vector3 m_vUpWorld;
	math::Vector3 m_vLookWorld;

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
