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
#include <Scene.h>
#include <Entity.h>
#include <CameraManager.h>
#include <CameraPerspective.h>
#include <ModelComponent.h>
#include <EntityManager.h>
#include <EngineEntityComponentFactory.h>

namespace hs {

/* CONSTRUCTOR - DESTRUCTOR */
EntityManager::EntityManager(he::gfx::Scene* scene) : m_Scene(scene)
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    entityMan->init();
    entityMan->installComponentFactory(NEW he::ge::EngineEntityComponentFactory());
}

EntityManager::~EntityManager()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_Entities.forEach([entityMan](he::ge::Entity* entity)
    {
        entityMan->destroyEntity(entity);
    });
    entityMan->destroy();
}

/* GENERAL */
he::ge::Entity* EntityManager::createEntity()
{
    using namespace he;

    // still some test code

    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    ge::Entity* newEntity(entityMan->createEmptyEntity());
    newEntity->setScene(m_Scene);

    ge::ModelComponent* modelComp(static_cast<ge::ModelComponent*>(
        entityMan->createComponent(he::HEFS::strModelComponent)));
    newEntity->addComponent(modelComp);
    modelComp->setModelMeshAndMaterial("testSceneBas.material", "box.binobj");    

    he::vec3 translation(m_Scene->getCameraManager()->getCamera("default")->getLook() * 10.0f);
    newEntity->setLocalTranslate(m_Scene->getCameraManager()->getCamera("default")->getPosition() + translation);

    newEntity->activate();

    HE_INFO("-- spawned new entity");

    m_Entities.add(newEntity);

    return newEntity;
}

} //end namespace