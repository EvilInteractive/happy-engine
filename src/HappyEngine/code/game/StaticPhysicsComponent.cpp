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
//Author:  Bastian Damman
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "StaticPhysicsComponent.h"

#include "Entity.h"
#include "PhysicsStaticActor.h"
#include "EntityComponentDesc.h"

namespace he {
namespace ge {
    
StaticPhysicsComponent::StaticPhysicsComponent(): m_StaticActor(nullptr), m_Parent(nullptr)
{
}


StaticPhysicsComponent::~StaticPhysicsComponent()
{
    HEDelete(m_StaticActor);
}

void StaticPhysicsComponent::init( Entity* pParent )
{
    m_Parent = pParent;
    m_StaticActor = HENew(px::PhysicsStaticActor)(getWorldMatrix());
}

void StaticPhysicsComponent::activate()
{
    HE_IF_ASSERT(m_StaticActor != nullptr, "activating none inited StaticPhysicsComponent")
    {
        m_StaticActor->attachToScene();
    }
}

void StaticPhysicsComponent::deactivate()
{
    HE_IF_ASSERT(m_StaticActor != nullptr, "deactivating none inited StaticPhysicsComponent")
    {
        m_StaticActor->detachFromScene();
    }
}

void StaticPhysicsComponent::addShape(  const px::IPhysicsShape* pShape, const px::PhysicsMaterial& material, uint32 collisionGroup, const mat44& localPose )
{
    HE_ASSERT(m_StaticActor != nullptr, "attach component first to entity");
    m_StaticActor->addShape(pShape, material, collisionGroup, localPose);
}

px::PhysicsStaticActor* StaticPhysicsComponent::getStaticActor() const
{
    return m_StaticActor;
}

void StaticPhysicsComponent::calculateWorldMatrix()
{
    Object3D::calculateWorldMatrix();
    if (m_StaticActor != nullptr)
        m_StaticActor->teleport(m_WorldMatrix);
}

void StaticPhysicsComponent::fillEntityComponentDesc( EntityComponentDesc& /*desc*/ )
{
    LOG(LogType_ProgrammerAssert, "Not implemented");
}

bool StaticPhysicsComponent::setProperty( const Property* const inProperty )
{
    return EntityComponent::setProperty(inProperty);
}

bool StaticPhysicsComponent::getProperty( Property* const inOutProperty )
{
    return EntityComponent::getProperty(inOutProperty);
}


} } //end namespace