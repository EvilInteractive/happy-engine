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

#include "PhysicsTrigger.h"

#include "Entity.h"
#include "Game.h"

namespace he {
namespace ge {

/* CONSTRUCTOR - DESTRUCTOR */
TriggerComponent::TriggerComponent() :  m_Trigger(nullptr),
                                        m_Parent(nullptr)
{
}

TriggerComponent::~TriggerComponent()
{
    delete m_Trigger;
}

/* ICOMPONENT */
void TriggerComponent::init(Entity* pParent)
{
    m_Parent = pParent;
    m_Trigger = NEW px::PhysicsTrigger(m_Parent->getWorldMatrix());
}

void TriggerComponent::serialize(SerializerStream& /*stream*/)
{

}

void TriggerComponent::deserialize(const SerializerStream& /*stream*/)
{

}

/* GENERAL */
void TriggerComponent::addShape(const px::IPhysicsShape* shape, uint32 collisionGroup, uint32 collisionGroupAgainst, 
    const mat44& localPose)
{
    HE_IF_ASSERT(m_Trigger != nullptr, "attach component first to entity")
    {
        m_Trigger->addTriggerShape(shape, collisionGroup, collisionGroupAgainst, localPose);
    }
}

void TriggerComponent::addOnTriggerEnterCallBack(boost::function<void()> callback)
{
    m_Trigger->addOnTriggerEnterCallBack(callback);
}

void TriggerComponent::addOnTriggerLeaveCallBack(boost::function<void()> callback)
{
    m_Trigger->addOnTriggerLeaveCallBack(callback);
}

/* GETTERS */
px::PhysicsTrigger* TriggerComponent::getTrigger()
{
    return m_Trigger;
}

void TriggerComponent::calculateWorldMatrix()
{
    Object3D::calculateWorldMatrix();
    m_Trigger->setPose(m_WorldMatrix);
}

} } //end namespace