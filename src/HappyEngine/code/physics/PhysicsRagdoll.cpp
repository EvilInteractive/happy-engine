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
//Created: 20/01/2012
#include "HappyPCH.h" 

#include "PhysicsRagdoll.h"

#include "PhysicsDynamicActor.h"
#include "PhysicsCapsuleShape.h"

#include "PhysicsEngine.h"

namespace he {
namespace px {

PhysicsRagdoll::PhysicsRagdoll()
{
}


PhysicsRagdoll::~PhysicsRagdoll()
{
}

void PhysicsRagdoll::initProperties( const RagdollDesc& desc, const PhysicsMaterial& material )
{
    m_pPelvis = new PhysicsDynamicActor(desc.pelvis.pose);
    PhysicsCapsuleShape pelvisShape(desc.pelvis.thickness / 2.0f, desc.pelvis.length);
    m_pPelvis->addShape(&pelvisShape, material, desc.pelvis.mass, mat44::Identity);

    m_pSpine2 = new PhysicsDynamicActor(desc.spine2.pose);
    PhysicsCapsuleShape spine2Shape(desc.spine2.thickness / 2.0f, desc.spine2.length);
    m_pSpine2->addShape(&spine2Shape, material, desc.spine2.mass, 
        mat44::createRotation(vec3(0, 0, 1), piOverTwo) *
        mat44::createTranslation(vec3(desc.spine2.length / 2, 0, 0)));

    m_pSpine1 = new PhysicsDynamicActor(desc.spine1.pose);
    PhysicsCapsuleShape spine1Shape(desc.spine1.thickness / 2.0f, desc.spine1.length);
    m_pSpine1->addShape(&spine1Shape, material, desc.spine1.mass, 
        mat44::createRotation(vec3(0, 0, 1), piOverTwo) *
        mat44::createTranslation(vec3(desc.spine1.length / 2, 0, 0)));

    m_pShoulders = new PhysicsDynamicActor(desc.shoulders.pose);
    PhysicsCapsuleShape shoulderShape(desc.shoulders.thickness / 2.0f, desc.shoulders.length);
    m_pShoulders->addShape(&shoulderShape, material, desc.shoulders.mass, mat44::Identity);


    //X = new PhysicsDynamicActor(mat44::createTranslation(desc.x.orig));
    //PhysicsCapsuleShape xShape(desc.x.thickness / 2.0f, desc.x.length);
    //X->addShape(&xShape, material, desc.x.mass, 
    //    mat44::createRotation(vec3(0, 0, 1), piOverTwo) *
    //    mat44::createTranslation(vec3(desc.x.length / 2, 0, 0)));
}

} } //end namespace