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

#include "EntityComponentDesc.h"
#include "Property.h"
#include "PropertyConverter.h"
#include "PropertyFeel.h"

namespace he {
namespace ge {
    
Entity::Entity(): m_Parent(nullptr), m_Scene(nullptr), m_Name("Entity"), m_IsActive(false)
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

void Entity::setName(const he::String& name)
{
    m_Name = name;
}

void Entity::setName(he::String&& name)
{
    m_Name = std::forward<he::String>(name);
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

void Entity::fillEntityComponentDesc( EntityComponentDesc& desc )
{
    desc.m_DisplayName = he::String("Entity");
    desc.m_ID = HEFS::strEntity;
    EntityComponent::fillEntityComponentDesc(desc);

    Property* nameProp(NEW Property());
    nameProp->init<he::String>(HEFS::strName, he::String(""));
    desc.m_Properties.setAt(nameProp->getName(), PropertyDesc(nameProp, "Name", "Sets the name of the component", 
        NEW PropertyConverterString(), NEW PropertyFeelDefault()));
}

bool Entity::setProperty( const Property* const inProperty )
{
    bool result(false);
    if (!EntityComponent::setProperty(inProperty))
    {
        const he::FixedString& name(inProperty->getName());
        if (name == HEFS::strName)
        {
            setName(inProperty->get<he::String>());
            result = true;
        }
    }
    else
    {
        result = true;
    }
    return result;
}

bool Entity::getProperty( Property* const inOutProperty )
{
    bool result(false);
    if (!EntityComponent::getProperty(inOutProperty))
    {
        const he::FixedString& name(inOutProperty->getName());
        if (name == HEFS::strName)
        {
            inOutProperty->set<he::String>(getName());
            result = true;
        }
    }
    else
    {
        result = true;
    }
    return result;
}

} } //end namespace
