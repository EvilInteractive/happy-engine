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
#include <LightComponent.h>
#include <LightManager.h>
#include <Font.h>
#include <OculusRiftBinding.h>
#include <ControlsManager.h>
#include <Canvas2D.h>
#include <Renderer2D.h>
#include <GlobalSettings.h>
#include <PostProcesser.h>
#include <ShadowCaster.h>
#include <ControlsManager.h>
#include <Mouse.h>

#include "FlyCamera.h"
#include "VRCamera.h"

ht::HappyPluginTestMain::HappyPluginTestMain()
    : m_View(nullptr)
    , m_Scene(nullptr)
    , m_RenderPipeline(nullptr)
    , m_Camera(nullptr)
    , m_DebugText(nullptr)
    , m_VREnabled(false)
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

    he::ct::ContentManager* contentMan(CONTENT);

    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(relViewport);

    m_RenderPipeline = NEW he::ge::DefaultRenderPipeline();
    m_RenderPipeline->init(m_View, m_Scene);

    m_VREnabled = he::GlobalSettings::getInstance()->getRenderSettings().stereoSetting == he::gfx::StereoSetting_OculusRift;

    if (m_VREnabled)
    {
        m_Camera = NEW VRCamera();
    }
    else
    {
        m_Camera = NEW FlyCamera();
    }
    m_Camera->setNearFarPlane(0.1f, 500.0f);

    m_View->setWindow(window, he::gfx::View::eViewInsertMode_First);
    m_View->init(cameraSettings);
    m_View->setCamera(m_Camera);
    if (m_View->getPostProcessor())
        m_RenderPipeline->get2DRenderer()->attachToRender(m_View->getPostProcessor());
    if (m_Scene->getShadowRenderer())
        m_RenderPipeline->get2DRenderer()->attachToRender(m_Scene->getShadowRenderer());

    he::gfx::LightManager* lightMan(m_Scene->getLightManager());
    lightMan->setDirectionalLight(he::normalize(he::vec3(0.5f, 1, 0.5f)), he::Color(1.0f, 0.95f, 0.9f), 1.5f);
    lightMan->setAmbientLight(he::Color(0.8f, 0.9f, 1.0f), 0.2f);

    he::gui::Font* const debugFont(contentMan->getDefaultFont(16));
    m_DebugText = NEW he::gui::Text();
    m_DebugText->setFont(debugFont);
    debugFont->release();
    m_RenderPipeline->get2DRenderer()->attachToRender(this);
}

void ht::HappyPluginTestMain::terminate()
{
    m_RenderPipeline->get2DRenderer()->detachFromRender(this);

    he::gfx::GraphicsEngine* const graphicsEngine(GRAPHICS);
    graphicsEngine->removeView(m_View);
    m_View = nullptr;
    graphicsEngine->removeScene(m_Scene);
    m_Scene = nullptr;
    delete m_Camera;
    m_Camera = nullptr;
    delete m_DebugText;
    m_DebugText = nullptr;

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
    // sceneModel->loadModelMeshAndMaterial("testSceneBas.material", "testScene3.binobj", "M_Scene");
    sceneModel->loadModelMeshAndMaterial("white.hm", "testScene.binobj", "M_Ground");
    // sceneModel->loadModelMeshAndMaterial("testSceneBas.material", "testScene2.binobj", "M_Ground");
    scene->activate();
    m_Entities.add(scene);

    int counters[3] = { 0, 0, 0 };
    for (size_t i(0); i < 50; ++i)
    {
        ge::Entity* const shape(entityMan->createEmptyEntity());
        shape->setScene(m_Scene);

        {
            ge::ModelComponent* const model(checked_cast<ge::ModelComponent*>(
                entityMan->createComponent(HEFS::strModelComponent)));
            shape->addComponent(model);
            const char* modelName(NULL);
            const char* materialName(NULL);
            const char* meshName(NULL);
            const char* name(NULL);
            const int obj(rand() % 3);
            switch (obj)
            {
            case 0: modelName = "testTheepot.binobj"; meshName = "Teapot001"; materialName = "theepot.hm"; name = "TheePot"; break;
            case 1: modelName = "cube.binobj"; meshName = "M_Cube"; materialName = "cube.hm"; name = "Cube"; break;
            case 2: modelName = "car.binobj"; meshName = "M_Car"; materialName = "car.hm";  name = "Car"; break;
            }
            model->loadModelMeshAndMaterial(materialName, modelName, meshName);

            const int nameSize(hesnprintf(nullptr, 0, "%s_%d", name, counters[obj]));
            he::String fullname;
            fullname.resize(nameSize+1);
            hesnprintf(&fullname[0], nameSize+1, "%s_%d", name, counters[obj]++);
            shape->setName(std::move(fullname));
        }

        {
            if (rand()%2 == 0)
            {
                ge::SpotLightComponent* const light(checked_cast<ge::SpotLightComponent*>(
                    entityMan->createComponent(HEFS::strSpotLightComponent)));
                shape->addComponent(light);
                light->setDirection(he::vec3(0, -1, 0));
                light->setAttenuation(he::vec2(3, 20));
                light->setColor(he::Color::fromHSB(rand()%360 / 360.0f, 0.8f, 1.0f));
                light->setMultiplier(1.0f);
            }
        }


        he::vec3 position(rand()%100 - 50.0f, 5.0f + rand()%10, rand()%100 - 50.0f);
        shape->setLocalTranslate(position);

        shape->activate();
        m_Entities.add(shape);
    }
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

void ht::HappyPluginTestMain::draw2D( he::gui::Canvas2D* canvas )
{
    if (m_VREnabled == true)
    {
        he::io::OculusRiftDevice* const device(CONTROLS->getOculusRiftBinding()->getDevice(0));
        if (device)
        {
            VRCamera* const camera(he::checked_cast<VRCamera*>(m_Camera));
            const he::vec3& velocityVector(camera->getVelocityVector());
            const he::vec3 pitchYawRoll(device->getPitchYawRoll());

            const he::mat44 velocityPitch(he::mat44::createRotation(he::vec3::right, -pitchYawRoll.x));
            const he::mat44 velocityRoll(he::mat44::createRotation(he::vec3::forward, -pitchYawRoll.z));
            const he::vec3 forceVector( velocityRoll * velocityPitch * he::vec3::forward );

            m_DebugText->clear();
            m_DebugText->addTextExt("TurnValue: %.2f\n"
                                    "Speed: %.2f\n"
                                    "Throttle: %.2f\n"
                                    "Direction: %.2f, %.2f, %.2f"
                , pitchYawRoll.z, camera->getSpeed(), camera->getThrottle()
                , velocityVector.x, velocityVector.y, velocityVector.z);

            canvas->setColor(he::Color(0.2f, 0.2f, 1.0f));
            canvas->fillText(*m_DebugText, he::vec2(300, 300));

            m_DebugText->clear();
            m_DebugText->addTextExt("Device: \n  Pitch: %.2f\n  Yaw: %.2f\n  Roll: %.2f"
                , he::toDegrees(pitchYawRoll.x), he::toDegrees(pitchYawRoll.y), he::toDegrees(pitchYawRoll.z));

            canvas->setColor(he::Color(0.2f, 0.2f, 1.0f));
            canvas->fillText(*m_DebugText, he::vec2(64, 400));
        }
    }
}
