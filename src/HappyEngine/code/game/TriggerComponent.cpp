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
#include "PhysicsUserData.h"

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
void TriggerComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "The parent of this component can not be nullptr!");
    m_Parent = parent;
    m_Trigger = NEW px::PhysicsTrigger(m_Parent->getWorldMatrix());
    m_Trigger->setUserData(m_Parent);
    
    he::eventCallback1<void, px::PhysicsActor*> onEnterHandler([&](px::PhysicsActor* actor)
    {
        const px::PhysicsUserData& data(actor->getUserData());
        if (RTTI::isA(data.getRTTI(), Entity::getRTTI()))
        {
            OnTriggerEnter(static_cast<Entity*>(data.getData()));
        }
        //else
        //{
        //   Should we handle this?
        //}
    });
    he::eventCallback1<void, px::PhysicsActor*> onLeaveHandler([&](px::PhysicsActor* actor)
    {
        const px::PhysicsUserData& data(actor->getUserData());
        if (RTTI::isA(data.getRTTI(), Entity::getRTTI()))
        {
            OnTriggerLeave(static_cast<Entity*>(data.getData()));
        }
        //else
        //{
        //   Should we handle this?
        //}
    });

    m_Trigger->OnTriggerEnter += onEnterHandler;
    m_Trigger->OnTriggerLeave += onLeaveHandler;
}

void TriggerComponent::activate()
{
    HE_IF_ASSERT(m_Trigger != nullptr, "activating none inited TriggerComponent")
    {
        m_Trigger->attachToScene();
    }
}

void TriggerComponent::deactivate()
{
    HE_IF_ASSERT(m_Trigger != nullptr, "deactivating none inited TriggerComponent")
    {
        m_Trigger->detachFromScene();
    }
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

void TriggerComponent::calculateWorldMatrix()
{
    Object3D::calculateWorldMatrix();
    m_Trigger->setPose(m_WorldMatrix);
}

} } //end namespace
