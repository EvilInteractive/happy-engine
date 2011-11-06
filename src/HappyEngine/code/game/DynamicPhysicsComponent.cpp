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
//Created: 29/10/2011
#include "StdAfx.h" 

#include "DynamicPhysicsComponent.h"
#include "HappyNew.h"
#include "HappyEngine.h"

#include "Entity.h"
#include "HappyEngine.h"
#include "Game.h"

namespace he {
namespace game {

DynamicPhysicsComponent::DynamicPhysicsComponent(): m_pDynamicActor(nullptr)
{
}


DynamicPhysicsComponent::~DynamicPhysicsComponent()
{
}

void DynamicPhysicsComponent::init( Entity* pParent )
{
    m_pParent = pParent;
    m_pDynamicActor = NEW px::PhysicsDynamicActor(m_pParent->getWorldMatrix());
    GAME->addToTickList(this);
    
    //if (HAPPYENGINE->isEditor())
    //    m_pDynamicActor->setKeyframed(true);
}

void DynamicPhysicsComponent::serialize(SerializerStream& /*stream*/)
{

}

void DynamicPhysicsComponent::deserialize(const SerializerStream& /*stream*/)
{

}

void DynamicPhysicsComponent::tick( float /*dTime*/ )
{
    m_pParent->setWorldMatrix(m_pDynamicActor->getPose());
}


void DynamicPhysicsComponent::addShape(  const px::IPhysicsShape* pShape, const px::PhysicsMaterial& material, float mass )
{
    ASSERT(m_pDynamicActor != nullptr, "attach component first to entity");
    m_pDynamicActor->addShape(pShape, material, mass);
}

px::PhysicsDynamicActor* DynamicPhysicsComponent::getDynamicActor() const
{
    return m_pDynamicActor;
}

} } //end namespace