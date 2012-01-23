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
//Author:  Sebastiaan Sprengers
//Created: 22/01/2012

#include "HappyPCH.h" 

#include "TriggerComponent.h"

#include "Entity.h"
#include "Game.h"

namespace he {
namespace game {

/* CONSTRUCTOR - DESTRUCTOR */
TriggerComponent::TriggerComponent() :  m_pTrigger(nullptr),
                                        m_pParent(nullptr)
{
}

TriggerComponent::~TriggerComponent()
{
    delete m_pTrigger;
    GAME->removeFromTickList(this);
}

/* ICOMPONENT */
void TriggerComponent::init(Entity* pParent)
{
    m_pParent = pParent;
    m_pTrigger = NEW px::PhysicsTrigger(m_pParent->getWorldMatrix());

    GAME->addToTickList(this);
}

void TriggerComponent::serialize(SerializerStream& /*stream*/)
{

}

void TriggerComponent::deserialize(const SerializerStream& /*stream*/)
{

}

/* ITICKABLE */
void TriggerComponent::tick(float /*dTime*/)
{
    m_pTrigger->setPose(m_pParent->getWorldMatrix());
}

/* GENERAL */
void TriggerComponent::addShape(const px::IPhysicsShape* pShape, const mat44& localPose)
{
    ASSERT(m_pTrigger != nullptr, "attach component first to entity");

    m_pTrigger->addTriggerShape(pShape, localPose);
}

void TriggerComponent::addOnTriggerEnterCallBack(boost::function<void()> callback)
{
    m_pTrigger->addOnTriggerEnterCallBack(callback);
}

void TriggerComponent::addOnTriggerLeaveCallBack(boost::function<void()> callback)
{
    m_pTrigger->addOnTriggerLeaveCallBack(callback);
}

/* GETTERS */
px::PhysicsTrigger* TriggerComponent::getTrigger()
{
    return m_pTrigger;
}

} } //end namespace