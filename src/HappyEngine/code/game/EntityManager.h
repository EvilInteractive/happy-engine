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

#ifndef _HE_EntityManager_H_
#define _HE_EntityManager_H_
#pragma once

#include "Singleton.h"
#include "EntityComponentType.h"
#include "Entity.h"

namespace he {
namespace ge {

class IEntityComponentFactory;
class Entity;
class EntityComponent;

class HAPPY_ENTRY EntityManager : public Singleton<EntityManager>
{
friend class Singleton<EntityManager>;
public:
    ~EntityManager();

    void init();
    void destroy();

    Entity* createEmptyEntity();
    void destroyEntity(Entity* const entity);

    EntityComponent* createComponent(const EntityComponentID& type);
    void destroyComponent(EntityComponent* const component);

    Entity* getEntity(const ObjectHandle& handle);
    EntityComponent* getComponent(const ObjectHandle& handle);

    void installComponentFactory(IEntityComponentFactory* const factory);

private:
    EntityManager();

    he::PrimitiveList<IEntityComponentFactory*> m_FactoryList;

    he::ObjectFactory<Entity> m_EntityFactory;
    he::ObjectFactory<EntityComponent, NoCreateObjectAllocator<EntityComponent>> m_ComponentFactory;

    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};

} } //end namespace

#endif
