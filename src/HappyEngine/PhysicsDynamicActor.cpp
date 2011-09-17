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
//Created: 22/08/2011
//Updated: 14/09/2011 -added addForce/velocity and kinematic methods

#include "PhysicsDynamicActor.h"
#include "HappyEngine.h"
#include "Assert.h"

namespace happyengine {
namespace physics {

PhysicsDynamicActor::PhysicsDynamicActor(const math::Vector3& position, const shapes::IPhysicsShape::pointer& shape, float density, PhysicsMaterial* pMaterial)
{
    m_pActor = PxCreateDynamic(*PHYSICS->getSDK(), 
                               PxTransform(PxVec3(position.x, position.y, position.z), PxQuat::createIdentity()), 
                               shape->getGeometry(), *pMaterial->getInternalMaterial(), density);
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    PHYSICS->getScene()->addActor(*m_pActor);
}
PhysicsDynamicActor::PhysicsDynamicActor(const math::Vector3& position, const std::vector<shapes::IPhysicsShape::pointer>& shapes, float density, PhysicsMaterial* pMaterial)
{  
    m_pActor = PHYSICS->getSDK()->createRigidDynamic(PxTransform(PxVec3(position.x, position.y, position.z)));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    std::for_each(shapes.cbegin(), shapes.cend(), [&](const shapes::IPhysicsShape::pointer& shape)
    {
        PxShape* pShape(m_pActor->createShape(shape->getGeometry(), *pMaterial->getInternalMaterial()));
        ASSERT(pShape != nullptr, "Shape creation failed");
    });

    PxRigidBodyExt::updateMassAndInertia(*m_pActor, density);

    PHYSICS->getScene()->addActor(*m_pActor);
}

PhysicsDynamicActor::~PhysicsDynamicActor()
{
    PHYSICS->getScene()->removeActor(*m_pActor);
    m_pActor->release();
}

math::Vector3 PhysicsDynamicActor::getPosition() const
{
    return math::Vector3(m_pActor->getGlobalPose().p);
}
math::Matrix PhysicsDynamicActor::getPose() const
{
    return math::Matrix(PxMat44(PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

void PhysicsDynamicActor::setVelocity(const math::Vector3& velocity)
{
    m_pActor->setLinearVelocity(physx::pubfnd3::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PhysicsDynamicActor::addVelocity(const math::Vector3& velocity)
{
    m_pActor->addForce(physx::pubfnd3::PxVec3(velocity.x, velocity.y, velocity.z), PxForceMode::eVELOCITY_CHANGE);
}
void PhysicsDynamicActor::addForce(const math::Vector3& force)
{
    m_pActor->addForce(physx::pubfnd3::PxVec3(force.x, force.y, force.z), PxForceMode::eFORCE);
}

void PhysicsDynamicActor::setKeyframed(bool keyframed)
{
    m_pActor->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, keyframed);
}
void PhysicsDynamicActor::keyframedSetPose(const math::Vector3& move, const math::Vector3& axis, float angle)
{
    m_pActor->moveKinematic(physx::pubfnd3::PxTransform(physx::pubfnd3::PxVec3(move.x, move.y, move.z),
        physx::pubfnd3::PxQuat(angle, physx::pubfnd3::PxVec3(axis.x, axis.y, axis.z))));
}

} } //end namespace