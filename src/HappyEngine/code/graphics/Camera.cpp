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
#include "StdAfx.h" 

#include "Camera.h"
#include "HappyNew.h"
#include "MathFunctions.h"

namespace he {
namespace gfx {

Camera::Camera(int viewportWidth, int viewportHeight) :	m_FOV(piOverFour),
                                                        m_AspectRatio(static_cast<float>(viewportHeight/viewportWidth)),
                                                        m_NearZ(0.1f),
                                                        m_FarZ(1000.0f),
                                                        m_matView(mat44::Identity),
                                                        m_matProjection(mat44::Identity),
                                                        m_matViewProjection(mat44::Identity),
                                                        m_vPosWorld(0.0f, 0.0f, 0.0f),
                                                        m_vRightWorld(0.0f, 0.0f, 1.0f),
                                                        m_vUpWorld(0.0f, 1.0f, 0.0f),
                                                        m_vLookWorld(1.0f, 0.0f, 0.0f),
                                                        m_vFxPosOffset(0.0f, 0.0f, 0.0f)
{
}

Camera::~Camera()
{
}

// GENERAL
void Camera::resize(int viewportWidth, int viewportHeight)
{
    m_AspectRatio = static_cast<float>(viewportHeight/viewportWidth);
}

void Camera::lookAt(const vec3 &pos, const vec3 &target, const vec3 &up)
{
    vec3 lookAt = target - pos;
    lookAt = normalize(lookAt);

    vec3 right = normalize(cross(lookAt, up));
    vec3 newUp = -normalize(cross(right, up));

    m_vPosWorld = pos;
    m_vRightWorld = right;
    m_vUpWorld = newUp;
    m_vLookWorld = lookAt;

    buildViewMatrix();
}

// SETTERS
void Camera::setPosition(const vec3 &pos)
{
    m_vPosWorld = pos;

    buildViewMatrix();
}
void Camera::setFxPositionOffset( const vec3& offset )
{
    m_vFxPosOffset = offset;
    buildViewMatrix();
}


void Camera::setLens(float aspectRatio, float fov, float nearZ, float farZ)
{
    m_FOV = fov;
    m_AspectRatio = aspectRatio;
    m_NearZ = nearZ;
    m_FarZ = farZ;

    buildProjectionMatrix();
}

void Camera::buildViewMatrix()
{
    vec3 pos(getPosition());
    m_matView = mat44::createLookAtLH(pos, pos + m_vLookWorld, m_vUpWorld);
    m_matViewProjection = m_matProjection * m_matView;
}

void Camera::buildProjectionMatrix()
{
    if (m_FOV > pi / 5 * 4.0f) m_FOV = static_cast<float>(pi / 5 * 4.0f);
    if (m_FOV < pi / 30.0f) m_FOV = static_cast<float>(pi / 30.0f);

    m_matProjection = mat44::createPerspectiveLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);

    m_matViewProjection = m_matProjection * m_matView;
}


// GETTERS
const mat44& Camera::getView() const
{ 
    return m_matView; 
}
const mat44& Camera::getProjection() const
{ 
    return m_matProjection; 
}
const mat44& Camera::getViewProjection() const
{ 
    return m_matViewProjection; 
}

vec3 Camera::getPosition() const
{
    vec3 posOffset(m_vRightWorld*m_vFxPosOffset.x + m_vUpWorld*m_vFxPosOffset.y + m_vLookWorld*m_vFxPosOffset.z);
    return m_vPosWorld + posOffset; 
}
const vec3& Camera::getRight() const
{ 
    return m_vRightWorld; 
}
const vec3& Camera::getUp() const
{ 
    return m_vUpWorld; 
}
const vec3& Camera::getLook() const
{ 
    return m_vLookWorld; 
}

float Camera::getNearClip() const
{
    return m_NearZ; 
}
float Camera::getFarClip() const
{
    return m_FarZ; 
}
float Camera::getFov() const
{ 
    return m_FOV; 
}
float Camera::getAspectRatio() const
{ 
    return m_AspectRatio; 
}

} } //end namespace