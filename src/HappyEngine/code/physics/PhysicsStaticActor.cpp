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
#include "Assert.h"

namespace he {
namespace px {

PhysicsStaticActor::PhysicsStaticActor(const mat44& pose, const shapes::IPhysicsShape::pointer& shape, const PhysicsMaterial::pointer& pMaterial)
{
    m_pActor = PHYSICS->getSDK()->createRigidStatic(PxTransform(pose.getPhyicsMatrix().column3.getXYZ(), 
                                                        PxQuat(physx::pubfnd3::PxMat33(pose.getPhyicsMatrix().column0.getXYZ(), pose.getPhyicsMatrix().column1.getXYZ(), 
                                                    pose.getPhyicsMatrix().column2.getXYZ()))));
    ASSERT(m_pActor != nullptr, "Actor creation failed");

    PxShape* pShape(m_pActor->createShape(shape->getGeometry(), *(pMaterial->getInternalMaterial())));
    ASSERT(pShape != nullptr, "Shape creation failed");

    PHYSICS->getScene()->addActor(*m_pActor);
}


PhysicsStaticActor::~PhysicsStaticActor()
{
    PHYSICS->getScene()->removeActor(*m_pActor);
    m_pActor->release();
}

vec3 PhysicsStaticActor::getPosition() const
{
    return vec3(m_pActor->getGlobalPose().p);
}
mat44 PhysicsStaticActor::getPose() const
{
    return mat44(PxMat44(PxMat33(m_pActor->getGlobalPose().q), m_pActor->getGlobalPose().p));
}

} } //end namespace