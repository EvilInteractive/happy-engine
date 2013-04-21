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
//Created: 2013/03/03

#include "HappyPluginTestPCH.h" 
#include "HappyPluginTestMain.h"

#include <StaticDataManager.h>
#include <GraphicsEngine.h>
#include <ContentManager.h>
#include <Window.h>
#include <Scene.h>
#include <View.h>
#include <DefaultRenderPipeline.h>

#include <EntityManager.h>
#include <Entity.h>
#include <ModelComponent.h>
#include <LightManager.h>

#include "FlyCamera.h"

ht::HappyPluginTestMain::HappyPluginTestMain()
    : m_View(nullptr)
    , m_Scene(nullptr)
    , m_RenderPipeline(nullptr)
    , m_Camera(nullptr)
{

}

ht::HappyPluginTestMain::~HappyPluginTestMain()
{

}

void ht::HappyPluginTestMain::init(he::gfx::Window* const window, const he::RectF& relViewport)
{
    HE_INFO("HappyPluginTestMain init");
    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    m_View = graphicsEngine->createView();
    m_Scene = graphicsEngine->createScene();

    he::gfx::RenderSettings settings;
    settings.stereoSetting = he::gfx::StereoSetting_None;
    settings.cameraSettings.setRelativeViewport(relViewport);
    CONTENT->setRenderSettings(settings);

    m_RenderPipeline = NEW he::ge::DefaultRenderPipeline();
    m_RenderPipeline->init(m_View, m_Scene, settings);

    m_Camera = NEW FlyCamera();
    m_Camera->setNearFarPlane(0.1f, 500.0f);

    m_View->setWindow(window, he::gfx::View::eViewInsertMode_First);
    m_View->init(settings);
    m_View->setCamera(m_Camera);

    he::gfx::LightManager* lightMan(m_Scene->getLightManager());
    lightMan->setDirectionalLight(he::normalize(he::vec3(0.5f, 1, 0.5f)), he::Color(1.0f, 0.95f, 0.9f), 2.0f);
    lightMan->setAmbientLight(he::Color(0.8f, 0.9f, 1.0f), 0.5f);
}

void ht::HappyPluginTestMain::terminate()
{
    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    graphicsEngine->removeView(m_View);
    m_View = nullptr;
    graphicsEngine->removeScene(m_Scene);
    m_Scene = nullptr;
    delete m_Camera;
    m_Camera = nullptr;

    delete m_RenderPipeline;
    m_RenderPipeline = nullptr;

    HE_INFO("HappyPluginTestMain terminated");
}

void ht::HappyPluginTestMain::onLoadLevel( const he::Path& /*path*/ )
{
    using namespace he;
    ge::EntityManager* const entityMan(ge::EntityManager::getInstance());
    ge::Entity* const scene(entityMan->createEmptyEntity());
    scene->setScene(m_Scene);
    ge::ModelComponent* const sceneModel(checked_cast<ge::ModelComponent*>(
        entityMan->createComponent(HEFS::strModelComponent)));
    scene->addComponent(sceneModel);
    sceneModel->setModelMeshAndMaterial("testSceneBas.material", "testScene3.binobj");
    scene->activate();
    m_Entities.add(scene);
}

void ht::HappyPluginTestMain::onUnloadLevel()
{
    using namespace he;
    ge::EntityManager* const entityMan(ge::EntityManager::getInstance());
    m_Entities.forEach([entityMan](ge::Entity* const entity)
    {
        entityMan->destroyEntity(entity);
    });
    m_Entities.clear();
}

void ht::HappyPluginTestMain::onLevelLoaded()
{

}

void ht::HappyPluginTestMain::onStartGame()
{

}

void ht::HappyPluginTestMain::onStopGame()
{

}

void ht::HappyPluginTestMain::onPauseGame()
{

}

void ht::HappyPluginTestMain::onResumeGame()
{
    m_View->setCamera(m_Camera);
}

he::gfx::ICamera* ht::HappyPluginTestMain::getActiveCamera() const
{
    return m_View->getCamera();
}

void ht::HappyPluginTestMain::setActiveCamera( he::gfx::ICamera* const camera )
{
    m_View->setCamera(camera);
}

