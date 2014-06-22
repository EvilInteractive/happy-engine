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
//Created: 30/09/2011
//Changed: 04/11/2011
#include "HappyPCH.h" 

#include "Entity.h"
#include "Scene.h"
#include "GraphicsEngine.h"

namespace he {
namespace ge {
    
Entity::Entity(): m_Parent(nullptr), m_Scene(nullptr), m_IsActive(false)
{
}


Entity::~Entity()
{
    HE_ASSERT(m_Components.empty(), "Destroying entity that still has components!");
}

void Entity::activate()
{
    HE_ASSERT(m_IsActive == false, "Entity is already active");
    m_IsActive = true;
    m_Components.forEach([](EntityComponent* const component)
    {
        component->activate();
    });
}

void Entity::deactivate()
{
    HE_ASSERT(m_IsActive == true, "Entity is already not active");
    m_IsActive = false;
    m_Components.forEach([](EntityComponent* const component)
    {
        component->deactivate();
    });
}


void Entity::init( Entity* parent )
{
    m_Parent = parent;
    m_Scene = m_Parent->getScene();
}

void Entity::addComponent( EntityComponent* const component )
{
    m_Components.add(component);
    attach(component);
    component->init(this);
    if (m_IsActive)
        component->activate();
}

void Entity::removeComponent( EntityComponent* const component )
{
    size_t index(0);
    if (m_Components.find(component, index))
    {
        removeComponentAt(index);
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Component not attached to Entity when removing!");
    }
}

void Entity::removeComponentAt( const size_t index )
{
    EntityComponent* const component(m_Components[index]);
    if (m_IsActive)
        component->deactivate();
    detach(component);
    m_Components.removeAt(index);
    component->setParent(nullptr);
}

EntityComponent* Entity::getComponent( const he::FixedString& id )
{
    size_t index(0);
    if (m_Components.find_if([&id](EntityComponent* const comp) { return comp->getComponentID() == id; }, index))
    {
        return m_Components[index];
    }
    else
    {
        return nullptr;
    }
}

} } //end namespace
