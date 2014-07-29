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
//Created: 05/03/2013

#include "HappySandBoxPCH.h" 

#include "EntityManager.h"

#include "Sandbox.h"
#include "runtime/SandboxEntityComponentFactory.h"
#include "runtime/EditorComponent.h"

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
    : m_EntityCreatedCallback(std::bind(&EntityManager::onEntityCreated, this, std::placeholders::_1))
    , m_EntityDestroyedCallback(std::bind(&EntityManager::onEntityDestroyed, this, std::placeholders::_1))
    , m_EntityDesc(nullptr)
    , m_EntityDesc(nullptr)
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
    entityMan->installComponentFactory(NEW SandboxEntityComponentFactory());

    he::PrimitiveList<he::ge::EntityComponentDesc*> descList;
    entityMan->fillComponentDescList(descList);
    descList.forEach([this](he::ge::EntityComponentDesc* const desc)
    {
        HE_INFO("Registering entity component: %s", desc->m_ID.c_str());
        HE_ASSERT(!m_ComponentDescList.find(desc->m_ID), "Component ID %s is already registered!", desc->m_ID.c_str());
        m_ComponentDescList[desc->m_ID] = desc;
    });

    HE_ASSERT(!m_EntityDesc, "EntityDesc is not nullptr when initializing!");
    m_EntityDesc = NEW he::ge::EntityComponentDesc();
    he::ge::Entity::fillEntityComponentDesc(*m_EntityDesc);
}

void EntityManager::destroy()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_Entities.forEach([entityMan](he::ge::Entity* entity)
    {
        entityMan->destroyEntity(entity);
    });
    entityMan->destroy();
    m_ComponentDescList.forEach([](const he::FixedString& /*key*/, he::ge::EntityComponentDesc* desc)
    {
        delete desc;
    });
    m_ComponentDescList.clear();

    delete m_EntityDesc;
    m_EntityDesc = nullptr;
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
    modelComp->loadModelMeshAndMaterial("testSceneBas.material", "box.binobj", "M_Box");    

    he::vec3 translation(camera->getLook() * 10.0f);
    newEntity->setLocalTranslate(camera->getPosition() + translation);

    newEntity->activate();

    HE_INFO("-- spawned new entity");

    m_Entities.add(newEntity);

    return newEntity;
}

void EntityManager::getComponentTypes( he::ObjectList<he::FixedString>& outList ) const
{
    m_ComponentDescList.forEach([&outList](const he::FixedString& id, he::ge::EntityComponentDesc* /*desc*/)
    {
        outList.add(id);
    });
}

he::ge::EntityComponentDesc* EntityManager::getComponentDescriptor( const he::FixedString& component ) const
{
    he::ge::EntityComponentDesc* const * desc = (component == he::HEFS::strEntity)? &m_EntityDesc : m_ComponentDescList.find(component);
    return desc? *desc : nullptr;
}

void EntityManager::onEntityCreated( he::ge::Entity* const entity )
{
    EditorComponent* const comp(he::checked_cast<EditorComponent*>(
        he::ge::EntityManager::getInstance()->createComponent(HSFS::strEditorComponent)));
    entity->addComponent(comp);
}

void EntityManager::onEntityDestroyed( he::ge::Entity* const /*entity*/ )
{
}

} //end namespace
