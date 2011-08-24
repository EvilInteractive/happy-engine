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

#include "PhysicsStaticActor.h"
#include "HappyEngine.h"
#include "Assert.h"

namespace happyengine {
namespace physics {

PhysicsStaticActor::PhysicsStaticActor(const math::Vector3 position, const shapes::IPhysicsShape& shape, PhysicsMaterial* pMaterial)
{
    m_pActor = PHYSICS->getSDK()->createRigidStatic(PxTransform(PxVec3(position.x, position.y, position.z)));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    PxShape* pShape(m_pActor->createShape(shape.getGeometry(), *pMaterial->getInternalMaterial()));
    ASSERT(pShape != nullptr, "Shape creation failed");

    PHYSICS->getScene()->addActor(*m_pActor);
}


PhysicsStaticActor::~PhysicsStaticActor()
{
    PHYSICS->getScene()->removeActor(*m_pActor);
    m_pActor->release();
}

math::Vector3 PhysicsStaticActor::getPosition() const
{
    return math::Vector3(m_pActor->getGlobalPose().p);
}
math::Matrix PhysicsStaticActor::getPose() const
{
    return math::Matrix(PxMat44(PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

} } //end namespace