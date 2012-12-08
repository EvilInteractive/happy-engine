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
//Created: 20/03/2012
#include "HappyBasTestPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ControlsManager.h"

#include "ModelMesh.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "Entity.h"
#include "ModelComponent.h"
#include "ContentManager.h"

#include "LightManager.h"

#include "LightComponent.h"
#include "DirectionalLight.h"
#include "Random.h"

#include "Scene.h"
#include "View.h"
#include "Window.h"
#include "DefaultRenderPipeline.h"

#include "IKeyboard.h"
#include "Renderer2D.h"
#include "ShapeRenderer.h"
#include "MessageBox.h"
#include "Canvas2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Font.h"
#include "SoundEngine.h"
#include "Sound2D.h"
#include "PostProcesser.h"
#include "WebView.h"
#include "WebListener.h"
#include "Canvas2Dnew.h"
#include "MathFunctions.h"
#include "Sprite.h"
#include "Gui.h"
#include "Player.h"
#include "PlayerThirdPersonCamera.h"

#include "InstancingController.h"
#include "InstancingManager.h"
#include "InstancedModelComponent.h"
#include "PhysicsEngine.h"
#include "StaticPhysicsComponent.h"
#include "PhysicsConcaveShape.h"
#include "PhysicsConvexShape.h"
#include "PhysicsBoxShape.h"
#include "DynamicPhysicsComponent.h"
#include "PhysicsDynamicActor.h"
#include "materialGenerator/MaterialGeneratorGraph.h"
#include "Mouse.h"
#include "Picker.h"

#define CONE_VERTICES 16
#define NUM_MOVING_ENTITIES 200

//#define ENABLE_WINDOW2

namespace ht {

he::Random MainGame::s_Random(0);

MainGame::MainGame()
    : m_FpsGraph(nullptr)
    , m_SpinShadows(false)
    , m_MovingEntityFase(0)
    , m_BackgroundSound(nullptr)
    , m_Window(nullptr), m_Window2(nullptr)
    , m_View(nullptr), m_View2(nullptr)
    , m_Scene(nullptr)
    , m_RenderPipeline(nullptr), m_RenderPipeline2(nullptr)
    , m_ToneMapGui(nullptr), m_ToneMapGuiListener(nullptr)
    , m_TestSprite(nullptr)
    , m_Player(nullptr)
    , m_MaterialGenerator(nullptr)
    , m_ColorTimer(0.0f)
{
    for (size_t i(0); i < NUM_MOVING_ENTITIES; ++i)
    {
        MovingEntityRandomness r;
        r.a = he::vec3(s_Random.nextFloat(-50, 50), s_Random.nextFloat(0, 50), s_Random.nextFloat(-50, 50));
        r.b = he::vec3(s_Random.nextFloat(-50, 50), s_Random.nextFloat(0, 50), s_Random.nextFloat(-50, 50));
        r.c = he::vec3((float)s_Random.nextInt(1, 5), (float)s_Random.nextInt(1, 5), (float)s_Random.nextInt(1, 5));
        m_MovingEntityRandomness.push_back(r);
    }

    for (he::uint8 r(0); r < 16; ++r)
        m_Colors[r].r(static_cast<he::uint8>(r * 16));
    for (he::uint8 g(0); g < 16; ++g)
        m_Colors[g].g(static_cast<he::uint8>( (16 - g) * 16));
    for (he::uint8 b(0); b < 8; ++b)
        m_Colors[b].b(static_cast<he::uint8>(b * 2 * 16));
    for (he::uint8 b(0); b < 8; ++b)
        m_Colors[b + 8].b(static_cast<he::uint8>((8 - b) * 2 * 16));
}


MainGame::~MainGame()
{
    PHYSICS->stopSimulation();

    delete m_MaterialGenerator;

    delete m_Player;

    m_RenderPipeline->get2DRenderer()->detachFromRender(m_FpsGraph);
    m_RenderPipeline->get2DRenderer()->detachFromRender(this);
    CONSOLE->detachFromRenderer();
    PROFILER->detachFromRenderer();

    delete m_FpsGraph;

    m_TestSprite->release();
    
    delete m_ToneMapGui;
    delete m_ToneMapGuiListener;

    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::ge::Entity* entity)
    {
        delete entity;     
    });

    delete m_RenderPipeline;
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeWindow(m_Window);

#ifdef ENABLE_WINDOW2
    GRAPHICS->removeView(m_View2);
    GRAPHICS->removeWindow(m_Window2);
    delete m_RenderPipeline2;
#endif

    m_Scene->getCameraManager()->deleteAllCameras();
    GRAPHICS->removeScene(m_Scene);
}

void MainGame::init()
{
    m_View = GRAPHICS->createView();
    m_Window = GRAPHICS->createWindow();

#ifdef ENABLE_WINDOW2
    m_View2 = GRAPHICS->createView3D();
    m_Window2 = GRAPHICS->createWindow();
#endif

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyBasTest");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();
}

void MainGame::load()
{
    using namespace he;

    #pragma region Window/View/Scene
    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;

    settings.lightingSettings.enableLighting = true;
    settings.lightingSettings.enableNormalMap = true;
    settings.lightingSettings.enableShadows = true;
    settings.lightingSettings.enableSpecular = true;

    settings.shadowSettings.shadowMult = 1;

    settings.postSettings.shaderSettings.enableAO = false;
    settings.postSettings.shaderSettings.enableBloom = true;
    settings.postSettings.shaderSettings.enableDepthEdgeDetect = false;
    settings.postSettings.shaderSettings.enableFog = true;
    settings.postSettings.shaderSettings.enableHDR = true;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;
    settings.postSettings.hdrSettings.exposureSpeed = 0.5f;

    CONTENT->setRenderSettings(settings);

    m_Scene = GRAPHICS->createScene();
    m_Scene->loadSkybox("engine/cubemaps/defaultSky.dds");

    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));

    m_RenderPipeline = NEW he::ge::DefaultRenderPipeline();
    m_RenderPipeline->init(m_View, m_Scene, settings);

    m_View->init(settings);

#ifdef ENABLE_WINDOW2
    m_Window2->setResizable(true);
    m_Window2->setVSync(false);
    m_Window2->setWindowDimension(720, 720);
    m_Window2->setWindowTitle("HappyBasTest - 2");
    m_Window2->create();

    m_View2 = NEW he::ge::DefaultRenderPipeline();
    m_View2->init(m_View, m_Scene, settings);
    m_View2->setWindow(m_Window2);
    m_View2->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View2->init(settings);
#endif
    
  
    FlyCamera* flyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", flyCamera);
    flyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 300.0f);
    flyCamera->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));
    m_View->setCamera(flyCamera);

#ifdef ENABLE_WINDOW2
    FlyCamera* flyCamera2 = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default2", flyCamera2);
    flyCamera2->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 1000.0f);
    flyCamera2->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));
    m_View2->setCamera(flyCamera2);
#endif
    
    m_FpsGraph = NEW tools::FPSGraph();
    m_FpsGraph->setType(tools::FPSGraph::Type_Full);
    m_FpsGraph->setPos(he::vec2(m_View->getViewport().width - 115.f, 5.f));

    CONSOLE->attachToRenderer(m_RenderPipeline->get2DRenderer());
    PROFILER->attachToRenderer(m_RenderPipeline->get2DRenderer());

    m_RenderPipeline->get2DRenderer()->attachToRender(this);
    m_RenderPipeline->get2DRenderer()->attachToRender(m_FpsGraph, 5);

    m_View->getPostProcessor()->setDebugRenderer(m_RenderPipeline->get2DRenderer());

#ifdef ENABLE_WINDOW2
    m_View2->getShapeRenderer()->attachToRenderer(this);
#endif
    
    #pragma endregion
    
    #pragma region Scene
    ge::Entity* scene(NEW ge::Entity());
    scene->init(m_Scene);
    ge::ModelComponent* modelComp(NEW ge::ModelComponent());
    scene->addComponent(modelComp);
    modelComp->setModelMeshAndMaterial("testScene3.material", "testPlatformer/scene.binobj");    
    m_EntityList.push_back(scene);
    ge::StaticPhysicsComponent* physicsComp(NEW ge::StaticPhysicsComponent());
    scene->addComponent(physicsComp);
    px::PhysicsConvexShape convexSceneShape("testPlatformer/scene.pxcv");
    px::PhysicsConcaveShape concaveSceneShape("testPlatformer/scene.pxcc");
    physicsComp->addShape(&convexSceneShape, px::PhysicsMaterial(1.2f, 1.0f, 0.1f));
    physicsComp->addShape(&concaveSceneShape, px::PhysicsMaterial(1.2f, 1.0f, 0.1f));

    m_Scene->getInstancingManager()->createController("bullet", true, "cube.material", "cube.binobj");
    m_Scene->getInstancingManager()->getController("bullet")->attachToScene(m_Scene);

    #pragma endregion
    
    #pragma region Lights
    m_Scene->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.3f);
    m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-4.0f, 5.f, 1.0f)), Color(1.0f, 0.9f, 0.8f, 1.0f), 0.0f);

    m_DebugSpotLight = m_Scene->getLightManager()->addSpotLight();
    m_DebugSpotLight->setLocalTranslate(vec3(-81.985f, -5.572f, 58.675f));
    m_DebugSpotLight->setDirection(vec3(0.0f, -0.809f, -0.5878f));
    m_DebugSpotLight->setMultiplier(5.0f);
    m_DebugSpotLight->setAttenuation(1.0f, 87.0f);
    m_DebugSpotLight->setFov(he::piOverTwo + he::pi / 6);
    m_DebugSpotLight->setColor(he::Color(1.0f, 1.0f, 1.0f));
    m_DebugSpotLight->setShadowResolution(gfx::ShadowResolution_512);

    he::gfx::SpotLight* spotlight = m_Scene->getLightManager()->addSpotLight();
    spotlight->setLocalTranslate(vec3(-32.468f, 10.291f, 34.99f));
    spotlight->setDirection(vec3(0.0f, -1.0f, 0.0f));
    spotlight->setMultiplier(2.0f);
    spotlight->setAttenuation(1.0f, 20.0f);
    spotlight->setFov(he::piOverTwo + he::pi / 6);
    spotlight->setColor(he::Color(1.0f, 1.0f, 1.0f));
    spotlight->setShadowResolution(gfx::ShadowResolution_256);

    he::gfx::PointLight* pointlight(m_Scene->getLightManager()->addPointLight());
    pointlight->setLocalTranslate(vec3(-81.98f, 5.572f, 45.634f));
    pointlight->setMultiplier(2.0f);
    pointlight->setAttenuation(0.0f, 38.0f);
    pointlight->setColor(he::Color(1.00f, 1.00f, 1.00f));

    #pragma endregion

    m_Player = NEW Player();
    m_View->setCamera(m_Player->getCamera());
    
    #pragma region GUI stuff
    gfx::Font* font(CONTENT->getDefaultFont(14));
    m_DebugText.setFont(font);
    font->release();

    font = CONTENT->loadFont("MODES.TTF", 128) ;
    m_BigText.setFont(font);
    m_BigText.setHorizontalAlignment(gui::Text::HAlignment_Left);
    m_BigText.setVerticalAlignment(gui::Text::VAlignment_Top);
    font->release();
    
    m_BackgroundSound = AUDIO->loadSound2D("stuff.wav", true);
    CONSOLE->registerCmd([this]()
    { 
        if (m_BackgroundSound->getState() != sfx::SOUND_STATE_PLAYING)
            m_BackgroundSound->play();
        else
            m_BackgroundSound->pause();
    }, "toggle_sound");
    

    /******* TONEMAP GUI *******/
    m_ToneMapGui = m_RenderPipeline->get2DRenderer()->createWebViewRelative(he::RectF(0,0,1,1), true);

    std::string guiPath(he::Path::getWorkingPath().getAbsolutePath(he::Path("../../data/gui/")).str());
    
    m_ToneMapGui->loadUrl(guiPath + "tonemap.html");
    m_ToneMapGui->setTransparent(true);

    m_ToneMapGuiListener = NEW he::gfx::WebListener(m_ToneMapGui);

    he::eventCallback1<void, const Awesomium::JSArray&> updateHandler(boost::bind(&ht::MainGame::updateToneMapData,this,_1));
    m_ToneMapGuiListener->addObjectCallback("HE", "updateTonemapData", updateHandler);

    // when gui loaded set slider val
    he::eventCallback0<void> onGuiLoaded([&]()
    {
        Awesomium::JSArray args;
        const he::gfx::ToneMapData& data(m_View->getPostProcessor()->getToneMapData());
        args.Push(Awesomium::JSValue(0));
        args.Push(Awesomium::JSValue(data.shoulderStrength));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(1));
        args.Push(Awesomium::JSValue(data.linearStrength));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(2));
        args.Push(Awesomium::JSValue(data.linearAngle));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(3));
        args.Push(Awesomium::JSValue(data.toeStrength));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(4));
        args.Push(Awesomium::JSValue(data.toeNumerator));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(5));
        args.Push(Awesomium::JSValue(data.toeDenominator));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
        args.Clear();

        args.Push(Awesomium::JSValue(6));
        args.Push(Awesomium::JSValue(data.exposureBias));
        m_ToneMapGuiListener->executeFunction("","setSliderValue", args);
    });

    m_ToneMapGui->OnUrlLoaded += onGuiLoaded;

    he::gui::SpriteCreator* creator(GUI->Sprites);
    m_TestSprite = creator->createSprite(he::vec2(200,200));
    creator->setActiveSprite(m_TestSprite);
    creator->roundedRectangle(he::vec2(0,0), he::vec2(200,200), 50.0f);
    creator->setColor(he::Color(1.0f,0.0f,1.0f));
    creator->fill();
    creator->roundedRectangle(he::vec2(40,0), he::vec2(40,40), 10.0f);
    creator->setColor(he::Color(1.0f,1.0f,1.0f));
    creator->fill();
    creator->roundedRectangle(he::vec2(120,0), he::vec2(40,40), 10.0f);
    creator->setColor(he::Color(1.0f,1.0f,1.0f));
    creator->fill();
    creator->renderSpriteAsync();
#pragma endregion

    m_MaterialGenerator = NEW he::tools::MaterialGeneratorGraph;
    m_MaterialGenerator->init();

    PHYSICS->startSimulation();
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);

    m_MovingEntityFase += dTime / 2.0f;
    if (m_MovingEntityFase >= he::twoPi)
        m_MovingEntityFase -= he::twoPi;

    he::io::IKeyboard* keyboard(CONTROLS->getKeyboard());
    if (CONTROLS->getFocus(this))
    {
        if (keyboard->isKeyPressed(he::io::Key_Return))
        {
            he::gfx::SpotLight* spotlight = m_Scene->getLightManager()->addSpotLight();
            spotlight->setLocalTranslate(m_View->getCamera()->getPosition());
            spotlight->setDirection(m_View->getCamera()->getLook());
            spotlight->setMultiplier(s_Random.nextFloat(1.0f, 5.0));
            spotlight->setAttenuation(1.0f, s_Random.nextFloat(10.0f, 20.0f));
            spotlight->setFov(he::piOverFour * s_Random.nextFloat(0.5f, 4.0f));
            he::vec3 color(he::normalize(he::vec3(s_Random.nextFloat(0, 1), s_Random.nextFloat(0, 1), s_Random.nextFloat(0, 1))));
            spotlight->setColor(he::Color(color.x, color.y, color.z, 1.0f));
            spotlight->setShadowResolution(he::gfx::ShadowResolution_256);
        }
        if (keyboard->isKeyDown(he::io::Key_Lctrl))
        {
            he::ge::Entity* bullet(NEW he::ge::Entity());
            bullet->init(m_Scene);
            bullet->setLocalTranslate(m_View->getCamera()->getPosition());
            he::ge::InstancedModelComponent* modelComp(NEW he::ge::InstancedModelComponent());
            modelComp->setLocalScale(he::vec3(0.5f));
            bullet->addComponent(modelComp);
            modelComp->setController("bullet");  
            he::ge::DynamicPhysicsComponent* physicsComp(NEW he::ge::DynamicPhysicsComponent());
            bullet->addComponent(physicsComp);
            he::px::PhysicsBoxShape shape(he::vec3(1.0f, 1.0f, 1.0f));
            physicsComp->addShape(&shape, he::px::PhysicsMaterial(1.0f, 0.8f, 0.3f), 1.0f, 0x00000001, 0xffffffff);
            m_EntityList.push_back(bullet);
            physicsComp->getDynamicActor()->setVelocity(m_View->getCamera()->getLook() * 40);
        }
        if (CONTROLS->getMouse()->isButtonPressed(he::io::MouseButton_Left))
        {
            //he::uint32 i(m_RenderPipeline->getPicker()->pick(CONTROLS->getMouse()->getPosition()));
            //
            //if (i != UINT32_MAX)
            //    ++i;
        }
        CONTROLS->returnFocus(this);
    }
    if (keyboard->isKeyPressed(he::io::Key_F9))
    {
        if (m_MaterialGenerator->isOpen())
            m_MaterialGenerator->close();
        else
            m_MaterialGenerator->open();
    }



    he::uint8 prevIndex(static_cast<he::uint8>(m_ColorTimer * 16));
    m_ColorTimer += dTime;
    he::uint8 newIndex(static_cast<he::uint8>(m_ColorTimer * 16));
    if (prevIndex != newIndex)
    {
        for (he::uint8 c(0); c < 8; ++c)
        {
            he::uint8 index(newIndex + c);
            if (index > 15)
                index -= 15;
            m_ShuffeledColor[c] = m_Colors[index];
        }
    }
    if (m_ColorTimer > 1.0f)
        m_ColorTimer -= static_cast<int>(m_ColorTimer);

    m_FpsGraph->tick(dTime);
}

void MainGame::drawShapes( he::gfx::ShapeRenderer* /*renderer*/ )
{

}

void MainGame::draw2D(he::gfx::Canvas2D* canvas)
{
    he::gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());

    he::gfx::CameraPerspective* camera(m_View->getCamera());
    const he::vec3& position(camera->getPosition());
    const he::vec3& look(camera->getLook());
    //const he::RectI& viewport(m_View->getViewport());

    m_RenderPipeline->getPicker()->drawDebug(canvas);

    canvas->setBlendStyle(he::gfx::BlendStyle_Opac);
    canvas->drawImage(m_DebugSpotLight->getShadowMap(), he::vec2(12, 300), he::vec2(128, 128));
    m_ToneMapGui->draw2D(canvas);
    
    m_DebugText.clear();
    m_DebugText.addTextExt("&0F0Position:&FFF %.2f, %.2f, %.2f\n", position.x, position.y, position.z);
    m_DebugText.addTextExt("&F00Look:&FFF %.2f, %.2f, %.2f\n", look.x, look.y, look.z);
    cvs->fillText(m_DebugText, he::vec2(12, 12));

    m_BigText.clear();
    m_BigText.addTextExt("&%c%c%cF&%c%c%ca&%c%c%cb&%c%c%cu&%c%c%cl&%c%c%co&%c%c%cu&%c%c%cs\n",
        m_ShuffeledColor[0].r16(), m_ShuffeledColor[0].g16(), m_ShuffeledColor[0].b16(),
        m_ShuffeledColor[1].r16(), m_ShuffeledColor[1].g16(), m_ShuffeledColor[1].b16(),
        m_ShuffeledColor[2].r16(), m_ShuffeledColor[2].g16(), m_ShuffeledColor[2].b16(),
        m_ShuffeledColor[3].r16(), m_ShuffeledColor[3].g16(), m_ShuffeledColor[3].b16(),
        m_ShuffeledColor[4].r16(), m_ShuffeledColor[4].g16(), m_ShuffeledColor[4].b16(),
        m_ShuffeledColor[5].r16(), m_ShuffeledColor[5].g16(), m_ShuffeledColor[5].b16(),
        m_ShuffeledColor[6].r16(), m_ShuffeledColor[6].g16(), m_ShuffeledColor[6].b16(),
        m_ShuffeledColor[7].r16(), m_ShuffeledColor[7].g16(), m_ShuffeledColor[7].b16());
    cvs->fillText(m_BigText, he::vec2(200,0));
    cvs->fillText(m_BigText, he::vec2(200,180));
    cvs->fillText(m_BigText, he::vec2(200,360));
    cvs->fillText(m_BigText, he::vec2(200,540));
    
    // NEW CANVAS TEST
    //he::gui::Canvas2Dnew* cvs = m_RenderPipeline->get2DRenderer()->getNewCanvas();
    //cvs->drawSprite(m_TestSprite, he::vec2(200,400), he::vec2(800,300));

    //canvas->drawImage(m_TestSprite->getRenderTexture(), he::vec2(12, 500));
}

void MainGame::updateToneMapData(const Awesomium::JSArray& args)
{
    int var(args[0].ToInteger());
    float value((float)args[1].ToDouble());

    switch(var)
    {
        case 0:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().shoulderStrength = value;
            break;
        }
        case 1:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().linearStrength = value;
            break;
        }
        case 2:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().linearAngle = value;
            break;
        }
        case 3:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().toeStrength = value;
            break;
        }
        case 4:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().toeNumerator = value;
            break;
        }
        case 5:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().toeDenominator = value;
            break;
        }
        case 6:
        {
            m_View->getPostProcessor()->getToneMapDataForEdit().exposureBias = value;
            break;
        }
    }
}

} //end namespace
