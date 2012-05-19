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
//Created: 09/12/2011
#include "HappyPCH.h" 

#include "InstancedModelComponent.h"
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "InstancingManager.h"
#include "InstancingController.h"
#include "DynamicBuffer.h"

namespace he {
namespace ge {

InstancedModelComponent::InstancedModelComponent(): m_InstanceId(0), m_ControllerKey("")
{
}


InstancedModelComponent::~InstancedModelComponent()
{
    if (m_ControllerKey != "")
    {
        GRAPHICS->getInstancingManager()->getController(m_ControllerKey)->removeInstance(m_InstanceId);
    }
}

void InstancedModelComponent::init( Entity* pParent )
{
    m_pParent = pParent;
}

void InstancedModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform << m_ControllerKey;
}

void InstancedModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform >> m_ControllerKey;
}

mat44 InstancedModelComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

void InstancedModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& InstancedModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void InstancedModelComponent::setController( const std::string& key )
{
    if (m_ControllerKey != "")
    {
        GRAPHICS->getInstancingManager()->getController(m_ControllerKey)->removeInstance(m_InstanceId);
    }
    else
    {
        m_ControllerKey = key;
        m_InstanceId = GRAPHICS->getInstancingManager()->getController(m_ControllerKey)->addInstance(this);
    }
}

const std::string& InstancedModelComponent::getController() const
{
    return m_ControllerKey;
}

void InstancedModelComponent::fillInstancingBuffer( gfx::DynamicBuffer& buffer ) const
{
    buffer.setValue(0, getWorldMatrix());
}



} } //end namespace