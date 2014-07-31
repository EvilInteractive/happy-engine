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
//Author:  Sebastiaan Sprengers
//Created: 31/08/2011
#include "HappyPCH.h" 

#include "CameraOrtho.h"
#include "Bound.h"
#include "GraphicsEngine.h"
#include "ShaderUniformBufferManager.h"

namespace he {
namespace gfx {

CameraOrtho::CameraOrtho() :
    m_NearZ(10.0f),
    m_FarZ(1000.0f),
    m_Left(-1), m_Right(1),
    m_Top(-1), m_Bottom(1),
    m_View(mat44::Identity),
    m_Projection(mat44::Identity),
    m_ViewProjection(mat44::Identity),
    m_PosWorld(0.0f, 0.0f, 0.0f),
    m_RightWorld(0.0f, 0.0f, 1.0f),
    m_UpWorld(0.0f, 1.0f, 0.0f),
    m_LookWorld(1.0f, 0.0f, 0.0f),
    m_RegenViewMatrix(false),
    m_RegenProjMatrix(false)
{
    m_Bound.calculate(this);
}

CameraOrtho::~CameraOrtho()
{
}

void CameraOrtho::lookAt(const vec3 &pos, const vec3 &target, const vec3 &up)
{
    vec3 lookAt = target - pos;
    lookAt = normalize(lookAt);

    vec3 right = normalize(cross(lookAt, up));
    vec3 newUp = -normalize(cross(right, up));

    m_PosWorld = pos;
    m_RightWorld = right;
    m_UpWorld = newUp;
    m_LookWorld = lookAt;

    m_RegenViewMatrix = true;
}

// SETTERS
void CameraOrtho::setPosition(const vec3 &pos)
{
    m_PosWorld = pos;

    m_RegenViewMatrix = true;
}

void CameraOrtho::setLens(float left, float right, float top, float bottom, float zNear, float zFar)
{
    m_Left = left;
    m_Right = right;
    m_Top = top;
    m_Bottom = bottom;
    m_NearZ = zNear;
    m_FarZ = zFar;

    m_RegenProjMatrix = true;
}

IntersectResult CameraOrtho::intersect( const Bound& bound ) const
{
    const Sphere& camSphereBound(m_Bound.getSphere());
    const Frustum& frustumBound(m_Bound.getFrustum());
    const Sphere& otherSphereBound(bound.getSphere());

    // Fast sphere - sphere test
    if (camSphereBound.intersectTest(otherSphereBound) == false) 
        return IntersectResult_Outside;

    // sphere frustum test
    switch(frustumBound.intersect(otherSphereBound))
    {
    case IntersectResult_Outside:
        return IntersectResult_Outside;
    case IntersectResult_Inside:
        return IntersectResult_Inside;
    case IntersectResult_Intersecting:
        switch(frustumBound.intersect(bound.getAABB()))
        {
        case IntersectResult_Outside:
            return IntersectResult_Outside;
        case IntersectResult_Inside:
            return IntersectResult_Inside;
        case IntersectResult_Intersecting:
            return IntersectResult_Intersecting;
        }
        break;
    }
    LOG(LogType_ProgrammerAssert, "Should never get here");
    return IntersectResult_Outside;
}

he::IntersectResult CameraOrtho::intersect( const Sphere& bound ) const
{
    const Sphere& camSphereBound(m_Bound.getSphere());
    const Frustum& frustumBound(m_Bound.getFrustum());
    const Sphere& otherSphereBound(bound);

    // Fast sphere - sphere test
    if (camSphereBound.intersectTest(otherSphereBound) == false) 
        return IntersectResult_Outside;

    // sphere frustum test
    return frustumBound.intersect(otherSphereBound);
}

void CameraOrtho::prepareForRendering()
{
    if (m_RegenViewMatrix)
    {
        vec3 pos(getPosition());
        m_View = mat44::createLookAtLH(pos, pos + m_LookWorld, m_UpWorld);
        m_ViewProjection = m_Projection * m_View;
    }
    if (m_RegenProjMatrix)
    {
        m_Projection = mat44::createOrthoLH(m_Left, m_Right, m_Top, m_Bottom, m_NearZ, m_FarZ);
        m_ViewProjection = m_Projection * m_View;
    }
    if (m_RegenProjMatrix || m_RegenViewMatrix)
    {
        m_ViewProjection = m_Projection * m_View;
        m_Bound.calculate(this);
        m_RegenViewMatrix = false;
        m_RegenProjMatrix = false;
    }
    GRAPHICS->getShaderUniformBufferManager()->updateCameraBuffer(this);
}

void CameraOrtho::setNearFarPlane( float nearZ, float farZ )
{
    m_NearZ = nearZ;
    m_FarZ = farZ;
    m_RegenProjMatrix = true;
}

void CameraOrtho::setAspectRatio( float aspectRatio )
{
    const float height(m_Bottom - m_Top);
    const float width(height * aspectRatio);
    const float diff(width - (m_Right - m_Left));
    m_Left -= diff / 2.0f;
    m_Right += diff / 2.0f;
    m_RegenProjMatrix = true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CameraOrtho::CameraBound::calculate(const CameraOrtho* cam)
{
    // Calc frustum
    const mat44& matrix(cam->getViewProjection());
    m_Frustum.getPlaneForEdit(FrustumPlane_LeftClipPlane) = Plane(
        matrix(3, 0) + matrix(0, 0),
        matrix(3, 1) + matrix(0, 1),
        matrix(3, 2) + matrix(0, 2),
        matrix(3, 3) + matrix(0, 3));

    m_Frustum.getPlaneForEdit(FrustumPlane_RightClipPlane) = Plane(
        matrix(3, 0) - matrix(0, 0),
        matrix(3, 1) - matrix(0, 1),
        matrix(3, 2) - matrix(0, 2),
        matrix(3, 3) - matrix(0, 3));

    m_Frustum.getPlaneForEdit(FrustumPlane_TopClipPlane) = Plane(
        matrix(3, 0) - matrix(1, 0),
        matrix(3, 1) - matrix(1, 1),
        matrix(3, 2) - matrix(1, 2),
        matrix(3, 3) - matrix(1, 3));

    m_Frustum.getPlaneForEdit(FrustumPlane_BottomClipPlane) = Plane(
        matrix(3, 0) + matrix(1, 0),
        matrix(3, 1) + matrix(1, 1),
        matrix(3, 2) + matrix(1, 2),
        matrix(3, 3) + matrix(1, 3));

    m_Frustum.getPlaneForEdit(FrustumPlane_NearClipPlane) = Plane(
        matrix(3, 0) + matrix(2, 0),
        matrix(3, 1) + matrix(2, 1),
        matrix(3, 2) + matrix(2, 2),
        matrix(3, 3) + matrix(2, 3));

    m_Frustum.getPlaneForEdit(FrustumPlane_FarClipPlane) = Plane(
        matrix(3, 0) - matrix(2, 0),
        matrix(3, 1) - matrix(2, 1),
        matrix(3, 2) - matrix(2, 2),
        matrix(3, 3) - matrix(2, 3));


    // calculate sphere
    float nearClip(cam->getNearClip());
    const vec3& pos(cam->getPosition());
    const vec3& look(cam->getLook());

    float viewLength(cam->getFarClip() - nearClip);

    float height(cam->getBottomClip() - cam->getTopClip());
    float width(cam->getRightClip() - cam->getLeftClip());

    vec3 halfWayPoint(0.0f, 0.0f, nearClip + viewLength * 0.5f);
    vec3 farCorner(width, height, viewLength);
    vec3 diff(halfWayPoint - farCorner);

    m_Sphere.setRadius(length(diff));
    m_Sphere.setPosition(pos + (look * ((viewLength * 0.5f) + nearClip)));
}

} } //end namespace