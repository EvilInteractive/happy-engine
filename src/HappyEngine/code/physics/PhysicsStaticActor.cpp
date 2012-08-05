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
//Created: 20/08/2011
#include "HappyPCH.h" 

#include "PhysicsStaticActor.h"
#include "PhysicsEngine.h"

#include "PhysicsBoxShape.h"
#include "PhysicsSphereShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsConcaveShape.h"
#include "PhysicsCapsuleShape.h"

#include "ResourceFactory.h"
#include "PhysicsConvexMesh.h"
#include "PhysicsConcaveMesh.h"

namespace he {
namespace px {

PhysicsStaticActor::PhysicsStaticActor(const mat44& pose, const IPhysicsShape* pShape, const PhysicsMaterial& material)
{
    PHYSICS->lock();
    m_Actor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_Actor != nullptr, "Actor creation failed");

    addShape(pShape, material);

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_Actor);
    PHYSICS->unlock();
}
PhysicsStaticActor::PhysicsStaticActor(const mat44& pose)
{  
    PHYSICS->lock();
    m_Actor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    PHYSICS->unlock();
    HE_ASSERT(m_Actor != nullptr, "Actor creation failed");

    PHYSICS->lock();
    PHYSICS->getScene()->addActor(*m_Actor);
    PHYSICS->unlock();
}
void PhysicsStaticActor::addShape( const IPhysicsShape* pShape, const PhysicsMaterial& material, 
                                   uint32 collisionGroup, const mat44& localPose)
{
    PHYSICS->lock();
    switch (pShape->getType())
    {
    case PhysicsShapeType_Box:
        {
            const PhysicsBoxShape* boxShape(static_cast<const PhysicsBoxShape*>(pShape));
            physx::PxShape* pxShape(m_Actor->createShape(
                physx::PxBoxGeometry(boxShape->getDimension().x / 2.0f, boxShape->getDimension().y / 2.0f, boxShape->getDimension().z / 2.0f), 
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup);
            break;
        }
    case PhysicsShapeType_Sphere:
        {
            const PhysicsSphereShape* sphereShape(static_cast<const PhysicsSphereShape*>(pShape));
            physx::PxShape* pxShape(m_Actor->createShape(
                physx::PxSphereGeometry(sphereShape->getRadius()), 
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup);
            break;
        }
    case PhysicsShapeType_Capsule:
        {
            const PhysicsCapsuleShape* capsuleShape(static_cast<const PhysicsCapsuleShape*>(pShape));
            physx::PxShape* pxShape(m_Actor->createShape(
                physx::PxCapsuleGeometry(capsuleShape->getRadius(), capsuleShape->getHeight() / 2.0f), 
                *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix())));
            addShape(pxShape, collisionGroup);
            break;
        }
        break;
    case PhysicsShapeType_Convex:
        {
            const PhysicsConvexShape* convexShape(static_cast<const PhysicsConvexShape*>(pShape));
            if (convexShape->getConvexMesh() != ObjectHandle::unassigned) // load failed
            {
                const std::vector<physx::PxConvexMesh*>& meshes(
                    ResourceFactory<PhysicsConvexMesh>::getInstance()->get(
                    convexShape->getConvexMesh())->getInternalMeshes());

                std::for_each(meshes.cbegin(), meshes.cend(), [&](physx::PxConvexMesh* mesh)
                {
                    physx::PxVec3 scale;
                    convexShape->getScale().toPxVec3(&scale);

                    physx::PxShape* pxShape(m_Actor->createShape(
                        physx::PxConvexMeshGeometry(mesh, 
                        physx::PxMeshScale(scale, physx::PxQuat::createIdentity())),
                        *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix())));
                    addShape(pxShape, collisionGroup);
                });
            }
            break;
        }
    case PhysicsShapeType_Concave:
        {
            const PhysicsConcaveShape* concaveShape(static_cast<const PhysicsConcaveShape*>(pShape));
            if (concaveShape->getConcaveMesh() != ObjectHandle::unassigned) // load failed
            {
                const std::vector<physx::PxTriangleMesh*>& meshes(
                    ResourceFactory<PhysicsConcaveMesh>::getInstance()->get(
                    concaveShape->getConcaveMesh())->getInternalMeshes());

                std::for_each(meshes.cbegin(), meshes.cend(), [&](physx::PxTriangleMesh* mesh)
                {
                    physx::PxVec3 scale;
                    concaveShape->getScale().toPxVec3(&scale);

                    physx::PxShape* pxShape(m_Actor->createShape(
                        physx::PxTriangleMeshGeometry(mesh, 
                        physx::PxMeshScale(scale, physx::PxQuat::createIdentity())),
                        *material.getInternalMaterial(), physx::PxTransform(localPose.getPhyicsMatrix())));
                    addShape(pxShape, collisionGroup);
                });
            }
            break;
        }

    default: 
        HE_ASSERT(false, "Unsupported type");
        break;
    }
    PHYSICS->unlock();
}

void PhysicsStaticActor::addShape( physx::PxShape* shape, uint32 collisionGroup )
{
    HE_ASSERT(shape != nullptr, "Shape creation failed");

    shape->userData = static_cast<IPhysicsActor*>(this);

    physx::PxFilterData filter;
    filter.word0 = collisionGroup;
    filter.word1 = 0xffffffff;

    shape->setQueryFilterData(filter);
    shape->setSimulationFilterData(filter);
}

PhysicsStaticActor::~PhysicsStaticActor()
{
    if (PHYSICS != nullptr)
    {
        PHYSICS->lock();
        PHYSICS->getScene()->removeActor(*m_Actor);
        PHYSICS->unlock();
        m_Actor->release();
    }
}

vec3 PhysicsStaticActor::getPosition() const
{
    return vec3(m_Actor->getGlobalPose().p);
}
mat44 PhysicsStaticActor::getPose() const
{
    return mat44(physx::PxMat44(physx::PxMat33(m_Actor->getGlobalPose().q), m_Actor->getGlobalPose().p));
}

physx::PxRigidActor* PhysicsStaticActor::getInternalActor() const
{
    return m_Actor;
}

} } //end namespace
