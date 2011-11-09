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

#include "StaticPhysicsComponent.h"
#include "HappyNew.h"
#include "HappyEngine.h"

#include "Entity.h"
#include "HappyEngine.h"
#include "Game.h"

namespace he {
namespace game {

StaticPhysicsComponent::StaticPhysicsComponent(): m_pStaticActor(nullptr)
{
}


StaticPhysicsComponent::~StaticPhysicsComponent()
{
	delete m_pStaticActor;
}

void StaticPhysicsComponent::init( Entity* pParent )
{
    m_pParent = pParent;
    m_pStaticActor = NEW px::PhysicsStaticActor(m_pParent->getWorldMatrix());
    
    //if (HAPPYENGINE->isEditor())
    //    m_pDynamicActor->setKeyframed(true);
}

void StaticPhysicsComponent::serialize(SerializerStream& /*stream*/)
{

}

void StaticPhysicsComponent::deserialize(const SerializerStream& /*stream*/)
{

}

void StaticPhysicsComponent::addShape(  const px::IPhysicsShape* pShape, const px::PhysicsMaterial& material )
{
    ASSERT(m_pStaticActor != nullptr, "attach component first to entity");
    m_pStaticActor->addShape(pShape, material);
}

px::PhysicsStaticActor* StaticPhysicsComponent::getStaticActor() const
{
    return m_pStaticActor;
}

} } //end namespace