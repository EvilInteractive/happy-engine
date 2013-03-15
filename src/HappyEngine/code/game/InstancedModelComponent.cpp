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
#include "Entity.h"
#include "Scene.h"
#include "GraphicsEngine.h"
#include "InstancingManager.h"
#include "InstancingController.h"
#include "DynamicBuffer.h"

namespace he {
namespace ge {

InstancedModelComponent::InstancedModelComponent(): m_InstanceId(UINT32_MAX), m_ControllerKey(""), m_Parent(nullptr)
{
}


InstancedModelComponent::~InstancedModelComponent()
{
    if (m_ControllerKey != "" && m_Parent != nullptr)
    {
        gfx::InstancingController* controller(m_Parent->getScene()->getInstancingManager()->getController(m_ControllerKey));
        HE_IF_ASSERT(controller != nullptr, "Instancing controller \"%s\" not found", m_ControllerKey.c_str())
        {
            controller->removeInstance(m_InstanceId);
        }
    }
}

void InstancedModelComponent::init( Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Parent cannot be nullptr!");
    HE_ASSERT(m_Parent == nullptr, "Do not init this component 2 times");
    m_Parent = parent;
    setController(m_ControllerKey);
}

void InstancedModelComponent::setController( const he::String& key )
{
    if (m_ControllerKey != "")
    {
        if (m_Parent != nullptr)
        {
            gfx::InstancingController* controller(m_Parent->getScene()->getInstancingManager()->getController(m_ControllerKey));
            HE_IF_ASSERT(controller != nullptr, "Instancing controller \"%s\" not found", m_ControllerKey.c_str())
            {
                controller->removeInstance(m_InstanceId);
            }
        }
        m_InstanceId = UINT32_MAX;
    }
    m_ControllerKey = key;
    if (key != "" && m_Parent != nullptr)
    {
        gfx::InstancingController* controller(m_Parent->getScene()->getInstancingManager()->getController(m_ControllerKey));
        HE_IF_ASSERT(controller != nullptr, "Instancing controller \"%s\" not found", m_ControllerKey.c_str())
        {
            m_InstanceId = controller->addInstance(this);
        }
    }
}

const he::String& InstancedModelComponent::getControllerKey() const
{
    return m_ControllerKey;
}

void InstancedModelComponent::fillInstancingBuffer( gfx::DynamicBuffer& buffer ) const
{
    buffer.setValue(0, getWorldMatrix());
}



} } //end namespace
