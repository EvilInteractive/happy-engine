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
//Created: 28/11/2012
#include "HappyPCH.h" 

#include "CharacterPhysicsComponent.h"

#include "Entity.h"
#include "Game.h"
#include "PhysicsCharacterController.h"

namespace he {
namespace ge {

CharacterPhysicsComponent::CharacterPhysicsComponent(): m_CharacterController(nullptr), m_Parent(nullptr)
{
}


CharacterPhysicsComponent::~CharacterPhysicsComponent()
{
    delete m_CharacterController;
}

void CharacterPhysicsComponent::init( Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Component must have a parent!");
    m_Parent = parent;
    m_CharacterController = NEW px::PhysicsCharacterController();
    m_CharacterController->setFootPosition(parent->getLocalTranslate());
    m_CharacterController->setGravity(vec3(0, -9.81f, 0));
    m_CharacterController->setHeight(2.0f);
    m_CharacterController->setRadius(0.5f);
    m_CharacterController->setMoveSpeed(vec3::zero);
}

void CharacterPhysicsComponent::activate()
{
    m_CharacterController->attachToScene();
    GAME->addToTickList(this);
}

void CharacterPhysicsComponent::deactivate()
{
    m_CharacterController->detachFromScene();
    GAME->removeFromTickList(this);
}

void CharacterPhysicsComponent::serialize(SerializerStream& /*stream*/)
{

}

void CharacterPhysicsComponent::deserialize(const SerializerStream& /*stream*/)
{

}

void CharacterPhysicsComponent::tick( float dTime )
{
    m_CharacterController->tick(dTime);
    const vec3 translation(m_CharacterController->getFootPosition());
    m_Parent->setLocalTranslate(translation);
}

} } //end namespace
