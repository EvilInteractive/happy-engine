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
//Author:  Sebastiaan Sprengers
//Created: 05/03/2013

#include "HappySandBoxPCH.h" 

#include "EntityManager.h"

#include "Sandbox.h"

#include <Scene.h>
#include <Entity.h>
#include <CameraManager.h>
#include <CameraPerspective.h>
#include <ModelComponent.h>
#include <EntityManager.h>
#include <EngineEntityComponentFactory.h>
#include <IPlugin.h>

namespace hs {

#pragma warning(disable:4355) // this in constructor
/* CONSTRUCTOR - DESTRUCTOR */
EntityManager::EntityManager() 
    : m_EntityCreatedCallback(boost::bind(&EntityManager::onEntityCreated, this, _1))
    , m_EntityDestroyedCallback(boost::bind(&EntityManager::onEntityDestroyed, this, _1))
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    entityMan->init();
    entityMan->EntityCreated += m_EntityCreatedCallback;
    entityMan->EntityDestroyed += m_EntityDestroyedCallback;
}
#pragma warning(default:4355)

EntityManager::~EntityManager()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    entityMan->EntityCreated -= m_EntityCreatedCallback;
    entityMan->EntityDestroyed -= m_EntityDestroyedCallback;
}


void EntityManager::init()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    entityMan->installComponentFactory(NEW he::ge::EngineEntityComponentFactory());

    he::PrimitiveList<he::ge::EntityComponentDesc*> descList;
    entityMan->fillComponentDescList(descList);
    descList.forEach([this](he::ge::EntityComponentDesc* const desc)
    {
        HE_INFO("Registering entity component: %s", desc->m_ID.c_str());
        HE_ASSERT(m_ComponentDescList.find(desc->m_ID) == m_ComponentDescList.cend(), "Component ID %s is already registered!", desc->m_ID.c_str());
        m_ComponentDescList[desc->m_ID] = desc;
    });
}

void EntityManager::destroy()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_Entities.forEach([entityMan](he::ge::Entity* entity)
    {
        entityMan->destroyEntity(entity);
    });
    entityMan->destroy();
    m_ComponentDescList.forEach([](std::pair<const he::FixedString, he::ge::EntityComponentDesc*>& desc)
    {
        delete desc.second;
    });
    m_ComponentDescList.clear();
}

/* GENERAL */
he::ge::Entity* EntityManager::createEntity()
{
    using namespace he;

    pl::IPlugin* const plugin(Sandbox::getInstance()->getGamePlugin());
    gfx::Scene* const scene(plugin->getScene());
    gfx::ICamera* const camera(plugin->getActiveCamera());

    // still some test code
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    ge::Entity* newEntity(entityMan->createEmptyEntity());
    newEntity->setScene(scene);

    ge::ModelComponent* modelComp(static_cast<ge::ModelComponent*>(
        entityMan->createComponent(he::HEFS::strModelComponent)));
    newEntity->addComponent(modelComp);
    modelComp->setModelMeshAndMaterial("testSceneBas.material", "box.binobj");    

    he::vec3 translation(camera->getLook() * 10.0f);
    newEntity->setLocalTranslate(camera->getPosition() + translation);

    newEntity->activate();

    HE_INFO("-- spawned new entity");

    m_Entities.add(newEntity);

    return newEntity;
}

void EntityManager::getComponentTypes( he::ObjectList<he::FixedString>& outList ) const
{
    m_ComponentDescList.forEach([&outList](const std::pair<const he::FixedString, he::ge::EntityComponentDesc*>& desc)
    {
        outList.add(desc.first);
    });
}

he::ge::EntityComponentDesc* EntityManager::getComponentDescriptor( const he::FixedString& component )
{
    HE_ASSERT(m_ComponentDescList.find(component) != m_ComponentDescList.cend(), "Could not find component %s", component.c_str());
    return m_ComponentDescList[component];
}

void EntityManager::onEntityCreated( he::ge::Entity* const /*entity*/ )
{
    HE_INFO("Logged a new entity!");
}

void EntityManager::onEntityDestroyed( he::ge::Entity* const /*entity*/ )
{
    HE_INFO("Destroyed an entity!");
}

} //end namespace