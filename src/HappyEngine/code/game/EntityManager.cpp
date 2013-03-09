//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/03/05
#include "HappyPCH.h" 

#include "EntityManager.h"

#include "Entity.h"
#include "EntityComponent.h"
#include "IEntityComponentFactory.h"

namespace he {
namespace ge {

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::init()
{
    m_EntityFactory.init(0, 10, "EntityFactory");
    m_ComponentFactory.init(0, 10, "ComponentFactory");
}

void EntityManager::destroy()
{
    HE_ASSERT(m_EntityFactory.isEmpty() == true, "Entity leak detected! (%d leaks)", m_EntityFactory.getSize());
    HE_ASSERT(m_ComponentFactory.isEmpty() == true, "EntityComponent leak detected! (%d leaks)", m_ComponentFactory.getSize());
    m_EntityFactory.destroyAll();
    m_ComponentFactory.destroyAll();
    m_FactoryList.forEach([](IEntityComponentFactory* const factory)
    {
        delete factory;
    });
}

void EntityManager::installComponentFactory( IEntityComponentFactory* const factory )
{
    m_FactoryList.add(factory);
}

Entity* EntityManager::createEmptyEntity()
{
    ObjectHandle handle(m_EntityFactory.create());
    Entity* const entity(m_EntityFactory.get(handle));
    return entity;
}

void EntityManager::destroyEntity( Entity* const entity )
{
    while (entity->getComponentCount() > 0)
    {
        EntityComponent* comp(entity->getComponentAt(0));
        entity->removeComponentAt(0);
        destroyComponent(comp);
    }
    m_EntityFactory.destroyObject(entity->getHandle());
}

Entity* EntityManager::getEntity( const ObjectHandle& handle )
{
    return m_EntityFactory.get(handle);
}

EntityComponent* EntityManager::createComponent( const EntityComponentType type )
{
    EntityComponent* result(nullptr);
    const size_t factories(m_FactoryList.size());
    for (size_t i(0); i < factories; ++i)
    {
        result = m_FactoryList[i]->createEntityComponent(type);
        if (result != nullptr)
            break;
    }
    HE_IF_ASSERT(result != nullptr, "Trying to create component for unregistered type: %s", type)
    {
        m_ComponentFactory.registerObject(result);
    }
    return result;
}

void EntityManager::destroyComponent( EntityComponent* const component )
{
    m_ComponentFactory.destroyObject(component->getHandle());
}

EntityComponent* EntityManager::getComponent( const ObjectHandle& handle )
{
    return m_ComponentFactory.get(handle);
}


} } //end namespace
