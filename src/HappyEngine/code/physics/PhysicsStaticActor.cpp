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

#include <PxRigidStatic.h>
#include <PxShape.h>

namespace he {
namespace px {

PhysicsStaticActor::PhysicsStaticActor(const mat44& pose, const IPhysicsShape* shape, const PhysicsMaterial& material)
{
    m_Actor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    HE_ASSERT(m_Actor != nullptr, "Actor creation failed");
    m_Actor->userData = static_cast<IPhysicsUserDataContainer*>(this);

    addShape(shape, material);
}
PhysicsStaticActor::PhysicsStaticActor(const mat44& pose)
{  
    m_Actor = PHYSICS->getSDK()->createRigidStatic(physx::PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
        physx::PxQuat(physx::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), 
        pose.getPhyicsMatrix().column1.getXYZ(), 
        pose.getPhyicsMatrix().column2.getXYZ()))));
    HE_ASSERT(m_Actor != nullptr, "Actor creation failed");
    m_Actor->userData = static_cast<IPhysicsUserDataContainer*>(this);
}
void PhysicsStaticActor::addShape( const IPhysicsShape* shape, const PhysicsMaterial& material, 
                                   uint32 collisionGroup, const mat44& localPose)
{
    he::PrimitiveList<physx::PxShape*> shapes;
    if (createShape(shapes, shape, material, localPose))
    {
        shapes.forEach([&](physx::PxShape* pxShape)
        {
            addShape(pxShape, collisionGroup);
        });
    }
}

void PhysicsStaticActor::addShape( physx::PxShape* shape, uint32 collisionGroup )
{
    HE_ASSERT(shape != nullptr, "Shape creation failed");

    shape->userData = static_cast<IPhysicsUserDataContainer*>(this);

    physx::PxFilterData filter;
    filter.word0 = collisionGroup;
    filter.word1 = 0xffffffff;

    shape->setQueryFilterData(filter);
    shape->setSimulationFilterData(filter);
}

PhysicsStaticActor::~PhysicsStaticActor()
{
    if (m_Actor != nullptr)
    {
        m_Actor->release();
    }
}

void PhysicsStaticActor::getTranslation( vec3& translation ) const
{
    const physx::PxVec3& pos(m_Actor->getGlobalPose().p);
    translation.x = pos.x;
    translation.y = pos.y;
    translation.z = pos.z;
}

void PhysicsStaticActor::getRotation( mat33& rotation ) const
{
    rotation = mat33(physx::PxMat33(m_Actor->getGlobalPose().q));
}

void PhysicsStaticActor::getPose(mat44& pose) const
{
    pose = mat44(physx::PxMat44(physx::PxMat33(m_Actor->getGlobalPose().q), m_Actor->getGlobalPose().p));
}

physx::PxRigidActor* PhysicsStaticActor::getInternalActor() const
{
    return m_Actor;
}

he::uint32 PhysicsStaticActor::getCompatibleShapes() const
{
    return PhysicsShapeType_StaticCompatible;
}

} } //end namespace
