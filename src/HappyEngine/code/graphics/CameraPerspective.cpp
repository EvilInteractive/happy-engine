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
//23/07/2012: Bastian Damman: added intersect

#include "HappyPCH.h" 

#include "CameraPerspective.h"
#include "Bound.h"

namespace he {
namespace gfx {

CameraPerspective::CameraPerspective() :	        
        m_FOV(piOverFour),
        m_AspectRatio(1),
        m_NearZ(10.0f),
        m_FarZ(1000.0f),
        m_View(mat44::Identity),
        m_Projection(mat44::Identity),
        m_ViewProjection(mat44::Identity),
        m_PosWorld(0.0f, 0.0f, 0.0f),
        m_RightWorld(0.0f, 0.0f, 1.0f),
        m_UpWorld(0.0f, 1.0f, 0.0f),
        m_LookWorld(1.0f, 0.0f, 0.0f),
        m_RegenViewMatrix(true), m_RegenProjMatrix(true)
{
    m_Bound.calculate(this);
}

CameraPerspective::~CameraPerspective()
{
}

// GENERAL
void CameraPerspective::lookAt(const vec3 &pos, const vec3 &target, const vec3 &up)
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
void CameraPerspective::setPosition(const vec3 &pos)
{
    m_PosWorld = pos;
    m_RegenViewMatrix = true;
}

void CameraPerspective::setLens(float aspectRatio, float fov, float nearZ, float farZ)
{
    m_FOV = fov;
    if (m_FOV > pi / 5 * 4.0f) m_FOV = static_cast<float>(pi / 5 * 4.0f);
    if (m_FOV < pi / 30.0f) m_FOV = static_cast<float>(pi / 30.0f);
    m_AspectRatio = aspectRatio;
    m_NearZ = nearZ;
    m_FarZ = farZ;

    m_RegenProjMatrix = true;
}

void CameraPerspective::setAspectRatio( float aspectRatio )
{
    if (fabs(m_AspectRatio - aspectRatio) > FLT_EPSILON)
    {
        m_AspectRatio = aspectRatio;
        m_RegenProjMatrix = true;
    }
}

he::IntersectResult CameraPerspective::intersect( const Bound& bound ) const
{
    const Sphere& camSphereBound(m_Bound.getSphere());
    const Cone& camConeBound(m_Bound.getCone());
    const Frustum& frustumBound(m_Bound.getFrustum());
    const Sphere& otherSphereBound(bound.getSphere());

    // Fast sphere - sphere test
    if (camSphereBound.intersectTest(otherSphereBound) == false) 
        return IntersectResult_Outside;

    // Fast cone - sphere test
    if (camConeBound.intersectTest(otherSphereBound) == false) 
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
    HE_ASSERT(false, "Should never get here");
    return IntersectResult_Outside;
}

he::IntersectResult CameraPerspective::intersect( const Sphere& bound ) const
{
    const Sphere& camSphereBound(m_Bound.getSphere());
    const Cone& camConeBound(m_Bound.getCone());
    const Frustum& frustumBound(m_Bound.getFrustum());
    const Sphere& otherSphereBound(bound);

    // Fast sphere - sphere test
    if (camSphereBound.intersectTest(otherSphereBound) == false) 
        return IntersectResult_Outside;

    // Fast cone - sphere test
    if (camConeBound.intersectTest(otherSphereBound) == false) 
        return IntersectResult_Outside;

    // sphere frustum test
    return frustumBound.intersect(otherSphereBound);
}

void CameraPerspective::prepareForRendering()
{
    if (m_RegenViewMatrix)
    {
        const vec3& pos(getPosition());
        m_View = mat44::createLookAtLH(pos, pos + m_LookWorld, m_UpWorld);
    }
    if (m_RegenProjMatrix)
    {
        m_Projection = mat44::createPerspectiveLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);
    }
    if (m_RegenProjMatrix || m_RegenViewMatrix)
    {
        m_ViewProjection = m_Projection * m_View;
        m_Bound.calculate(this);
        m_RegenViewMatrix = false;
        m_RegenProjMatrix = false;
    }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CameraPerspective::CameraBound::calculate(const CameraPerspective* cam)
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
    float farClip(cam->getFarClip());
    float tanFov(tan(cam->getFov() * 0.5f));
    const vec3& pos(cam->getPosition());
    const vec3& look(cam->getLook());

    float viewLength(farClip - nearClip);

    float halfHeight(farClip * tanFov);
    float halfWidth(halfHeight * cam->getAspectRatio());

    vec3 halfWayPoint(0.0f, 0.0f, viewLength * 0.5f);
    vec3 farCorner(halfWidth, halfHeight, viewLength);
    vec3 diff(halfWayPoint - farCorner);

    m_Sphere.setRadius(length(diff));
    m_Sphere.setPosition(pos + look * halfWayPoint.z);


    // calculate cone
    float corner(sqrt(halfWidth * halfWidth + halfHeight * halfHeight));
    float fov(atan(corner / farClip) * 2.0f);

    m_Cone.setPosition(pos);
    m_Cone.setAxis(look);
    m_Cone.setFov(fov);
    m_Cone.setAxisLength(farClip);
}

} } //end namespace