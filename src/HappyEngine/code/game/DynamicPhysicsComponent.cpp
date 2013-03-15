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
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "DynamicPhysicsComponent.h"

#include "Entity.h"
#include "Game.h"
#include "PhysicsDynamicActor.h"

namespace he {
namespace ge {
    
DynamicPhysicsComponent::DynamicPhysicsComponent(): m_DynamicActor(nullptr), m_Parent(nullptr)
{
}


DynamicPhysicsComponent::~DynamicPhysicsComponent()
{
    delete m_DynamicActor;
}

void DynamicPhysicsComponent::init( Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Component must have a parent!");
    m_Parent = parent;
    m_DynamicActor = NEW px::PhysicsDynamicActor(m_Parent->getWorldMatrix());
}

void DynamicPhysicsComponent::activate()
{
    HE_IF_ASSERT(m_DynamicActor != nullptr, "activating none inited DynamicPhysicsComponent")
    {
        m_DynamicActor->attachToScene();
        GAME->addToTickList(this);
    }
}

void DynamicPhysicsComponent::deactivate()
{
    HE_IF_ASSERT(m_DynamicActor != nullptr, "deactivating none inited DynamicPhysicsComponent")
    {
        GAME->removeFromTickList(this);
        m_DynamicActor->detachFromScene();
    }
}

void DynamicPhysicsComponent::tick( float /*dTime*/ )
{
    vec3 translation;
    mat33 rotation;
    m_DynamicActor->getTranslation(translation);
    m_DynamicActor->getRotation(rotation);
    m_Parent->setLocalTranslate(translation);
    m_Parent->setLocalRotate(rotation);
}


void DynamicPhysicsComponent::addShape(  const px::IPhysicsShape* shape, const px::PhysicsMaterial& material, float mass, 
    uint32 collisionGroup, uint32 collisionGroupAgainst, const mat44& localPose )
{
    HE_ASSERT(m_DynamicActor != nullptr, "attach component first to entity");
    m_DynamicActor->addShape(shape, material, mass, collisionGroup, collisionGroupAgainst, localPose);
}

px::PhysicsDynamicActor* DynamicPhysicsComponent::getDynamicActor() const
{
    return m_DynamicActor;
}

void DynamicPhysicsComponent::calculateWorldMatrix()
{
    Object3D::calculateWorldMatrix();
}

} } //end namespace
