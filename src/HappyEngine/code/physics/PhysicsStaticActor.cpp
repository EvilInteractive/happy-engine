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
//Author:  Bastian Damman
//Created: 20/08/2011
#include "StdAfx.h" 

#include "PhysicsStaticActor.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsConcaveShape.h"

#include "vehicle/PxVehicle.h"
#include "vehicle/PxVehicleUtils.h"

namespace he {
namespace px {

PhysicsStaticActor::PhysicsStaticActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material)
{
    m_pActor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    addShape(pShape, material);

    PHYSICS->getScene()->addActor(*m_pActor);
}
PhysicsStaticActor::PhysicsStaticActor(const mat44& pose)
{  
    m_pActor = PHYSICS->getSDK()->createRigidStatic(physx::pubfnd3::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::pubfnd3::PxQuat(physx::pubfnd3::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    PHYSICS->getScene()->addActor(*m_pActor);
}
void PhysicsStaticActor::addShape( const IPhysicsShape* pShape, const PhysicsMaterial& material) const
{
    physx::PxShape* pPxShape(nullptr);
    switch (pShape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* pBoxShape(dynamic_cast<const PhysicsBoxShape*>(pShape));
            ASSERT(pBoxShape != nullptr, "IPhysicsShape type PhysicsShapeType_Box is not a PhysicsBoxShape");
            pPxShape = m_pActor->createShape(
                physx::PxBoxGeometry(pBoxShape->getDimension().x / 2.0f, pBoxShape->getDimension().y / 2.0f, pBoxShape->getDimension().z / 2.0f), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* pSphereShape(dynamic_cast<const PhysicsSphereShape*>(pShape));
            ASSERT(pSphereShape != nullptr, "IPhysicsShape type PhysicsShapeType_Sphere is not a PhysicsSphereShape");
            pPxShape = m_pActor->createShape(
                physx::PxSphereGeometry(pSphereShape->getRadius()), 
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Capsule:
        ASSERT(false, "Not Implemented");
        break;
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* pConvexShape(dynamic_cast<const PhysicsConvexShape*>(pShape));
            ASSERT(pConvexShape != nullptr, "IPhysicsShape type PhysicsShapeType_Convex is not a PhysicsConvexShape");
            pPxShape = m_pActor->createShape(
                physx::PxConvexMeshGeometry(pConvexShape->getInternalMesh(), 
                physx::PxMeshScale(physx::pubfnd3::PxVec3(pConvexShape->getScale().x, pConvexShape->getScale().y, pConvexShape->getScale().z), physx::PxQuat::createIdentity())),
                *material.getInternalMaterial());
            break;
        }
    case PhysicsShapeType_Concave:
        {
            const PhysicsConcaveShape* pConcaveShape(dynamic_cast<const PhysicsConcaveShape*>(pShape));
            ASSERT(pConcaveShape != nullptr, "IPhysicsShape type PhysicsShapeType_Concave is not a PhysicsConcaveShape");
            pPxShape = m_pActor->createShape(
                physx::PxTriangleMeshGeometry(pConcaveShape->getInternalMesh(), 
                physx::PxMeshScale(physx::pubfnd3::PxVec3(pConcaveShape->getScale().x, pConcaveShape->getScale().y, pConcaveShape->getScale().z), physx::PxQuat::createIdentity())),
                *material.getInternalMaterial());
            break;
        }

    default: 
        ASSERT(false, "Not Implemented");
        break;
    }
    ASSERT(pPxShape != nullptr, "Shape creation failed");

    physx::PxFilterData sFilter;
    sFilter.word0 = COLLISION_FLAG_GROUND;
    sFilter.word1 = COLLISION_FLAG_GROUND_AGAINST;
    physx::PxFilterData qFilter;
    physx::PxSetupDrivableShapeQueryFilterData(&qFilter);

    pPxShape->setQueryFilterData(qFilter);
    pPxShape->setSimulationFilterData(sFilter);
}

PhysicsStaticActor::~PhysicsStaticActor()
{
    if (PHYSICS != nullptr)
    {
        PHYSICS->getScene()->removeActor(*m_pActor);
        m_pActor->release();
    }
}

vec3 PhysicsStaticActor::getPosition() const
{
    return vec3(m_pActor->getGlobalPose().p);
}
mat44 PhysicsStaticActor::getPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

} } //end namespace