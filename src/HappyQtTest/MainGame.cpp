//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "MainGame.h"

#include <vector>
#include <set>
#include <list>
#include <sstream>
#include <iostream>

#include "mat44.h"
#include "vec3.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "MathFunctions.h"
#include "NetworkManager.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Console.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"
#include "PhysicsEngine.h"
#include "PhysicsBoxShape.h"
#include "SoundEngine.h"

#include "IniReader.h"
#include "FileNotFoundException.h"
#include "Circle.h"

#include "HappyNew.h"

#include "boost/timer.hpp"

#include "Random.h"
#include "SimpleForward3DRenderer.h"

#include "BinaryStream.h"

#include "ModelComponent.h"
#include "StaticPhysicsComponent.h"
#include "DynamicPhysicsComponent.h"
#include "PhysicsConvexShape.h"
#include "PhysicsConcaveShape.h"

#include "PhysicsCarManager.h"
#include "LightManager.h"

#include "Profiler.h"

#include "PhysicsData.h"

#include "Deferred3DRenderer.h"
#include "CameraManager.h"

namespace happytest {

MainGame::MainGame(/*qt::*/QWidget *parent) : he::gfx::HappyQtWidget(parent), m_pTestObject(nullptr), m_BackgroundIndex(0),
                       m_DrawTimer(0), m_UpdateTimer(0),       
                       m_pFPSGraph(NEW he::tools::FPSGraph()),
                       m_pCurrentCamera(nullptr), m_pTestButton(nullptr), m_pAxis(nullptr),
                       m_pTextBox(nullptr), m_bTest(true), m_bTest2(true), m_Test3("You can edit this string via console"),
                       m_pScene(0), m_pSky(0),
                       m_pTestSound2D(nullptr), m_pTestGrid(nullptr),
                       m_pFrictionTable(nullptr), m_pFxEditorBinding(NEW he::tools::HappyFxEditorBinding())
{
    using namespace he;
    m_BackgroundColors[0] = Color((byte)10, (byte)130, (byte)131, (byte)255);
    m_BackgroundColors[1] = Color((byte)122, (byte)186, (byte)122, (byte)255);
    m_BackgroundColors[2] = Color((byte)255, (byte)127, (byte)80, (byte)255);
    m_BackgroundColors[3] = Color((byte)255, (byte)165, (byte)0, (byte)255);
    m_BackgroundColors[4] = Color((byte)30, (byte)144, (byte)255, (byte)255);
}


MainGame::~MainGame()
{
    delete m_pFollowCamera;
    delete m_pFlyCamera;

    delete m_pTestObject;
    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](he::game::Entity* pBullet)
    {
        delete pBullet;
    });


    delete m_pFPSGraph;

    delete m_pTestButton;
    delete m_pAxis;
    delete m_pScene;
    delete m_pSky;
    delete m_pTextBox;
    delete m_pTestButton2;
    delete m_pTestButton3;
    delete m_pTestGrid;

    delete m_pFrictionTable;

    delete m_pFxEditorBinding;

    NETWORK->stop();
}

void MainGame::init()
{
    using namespace he;
    io::IniReader reader;
    try { reader.open("../data/settings.ini"); }
    catch (err::FileNotFoundException& e)
    { std::wcout << e.getMsg() << "\n"; }
    if (reader.isOpen())
    {
        vec2 windowDim(reader.readVector2(L"Window", L"dimension", vec2(1280, 720)));
        GRAPHICS->setScreenDimension(static_cast<int>(windowDim.x), static_cast<int>(windowDim.y));
        GRAPHICS->setWindowTitle(reader.readString(L"Window", L"title", "Test"));

        vec4 bColor(reader.readVector4(L"Background", L"color"));
        GRAPHICS->setBackgroundColor(Color(bColor.x, bColor.y, bColor.z, bColor.w));

        GRAPHICS->setVSync(reader.readBool(L"Graphics", L"vsync", true));
        GRAPHICS->toggleFullscreen(reader.readBool(L"Graphics", L"fullscreen"));
    }
}

void MainGame::load()
{
    using namespace he;

    //happycooker::HappyCooker* cooker(happycooker::HappyCooker::getInstance());
    //cooker->cookToBinObj("D:/File Server/Programming/happy-engine/art/models/export/car.dae", "D:/File Server/Programming/happy-engine/src/HappyTest/data/models/car.binobj");


    using namespace he;

    m_SplashImage = CONTENT->asyncLoadTexture("happy_splash.png");

    //////////////////////////////////////////////////////////////////////////
    /// PHYSICS
    //////////////////////////////////////////////////////////////////////////
    m_pFrictionTable = NEW he::px::DriveableSurfaceTyreFrictionTable(MAX_DRIVEABLE_MATERIALS, MAX_TYRE_TYPES);

    m_pFrictionTable->setMaterial(DM_Grass,     px::PhysicsMaterial(0.90f, 0.85f, 0.05f));
    m_pFrictionTable->setMaterial(DM_Sand,      px::PhysicsMaterial(0.75f, 0.70f, 0.01f));
    m_pFrictionTable->setMaterial(DM_Wood,      px::PhysicsMaterial(0.95f, 0.90f, 0.09f));
    m_pFrictionTable->setMaterial(DM_Ice,       px::PhysicsMaterial(0.65f, 0.60f, 0.09f));
    m_pFrictionTable->setMaterial(DM_Tarmac,    px::PhysicsMaterial(1.40f, 1.30f, 0.01f));
    m_pFrictionTable->setMaterial(DM_Concrete,  px::PhysicsMaterial(1.50f, 1.40f, 0.01f));
    m_pFrictionTable->setMaterial(DM_Tyre,      px::PhysicsMaterial(1.30f, 1.20f, 0.10f));
    m_pFrictionTable->setMaterial(DM_Metal,     px::PhysicsMaterial(0.85f, 0.80f, 0.08f));

    m_pFrictionTable->setTyreType(TT_Slicks, 1.0f);
    m_pFrictionTable->setTyreType(TT_Wets, 1.2f);

    PHYSICS->getCarManager()->init(m_pFrictionTable);
    PHYSICS->startSimulation();

    gfx::PointLight::pointer pPlight(GRAPHICS->getLightManager()->addPointLight(vec3(0, 2, 0), Color((byte)255, 50, 50, 255), 5.0f, 1, 10));
    //m_pSpotLight = GRAPHICS->getLightManager()->addSpotLight(vec3(-1, 0, -1), vec3(-1, 0, 0), Color((byte)255, 255, 200, 255), 3.0f, piOverFour, 1, 30);

    //Random r;
    //for (int i = 0; i < 5; ++i)
    //{
    //    vec3 color(r.nextFloat(0.0f, 1.0f), r.nextFloat(0.0f, 1.0f), r.nextFloat(0.0f, 1.0f));
    //    color = normalize(color);
    //    GRAPHICS->getLightManager()->addPointLight(vec3(r.nextFloat(0, -100), r.nextFloat(3, 7), r.nextFloat(0, 100)), 
    //                                Color(color.x, color.y, color.z, 1.0f), r.nextFloat(5, 10), 1, r.nextFloat(10, 30));
    //}

    m_pCarLight = GRAPHICS->getLightManager()->addPointLight(vec3(), Color(1.0f, 0.8f, 0.5f), 5, 1, 30);

       //GRAPHICS->getLightManager()->addSpotLight(vec3(r.nextFloat(0, -100), r.nextFloat(5, 20), r.nextFloat(0, 100)), vec3(0, -1, 0), Color((byte)255, 255, 200, 255), 1.0f, piOverTwo, 1, 20);
    //GRAPHICS->getLightManager()->setDirectionalLight(vec3(0, 1, 0), Color((byte)150, 200, 255, 255), 20.0f);
    GRAPHICS->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 10.0f);
    GRAPHICS->getLightManager()->setDirectionalLight(normalize(vec3(-0.5f, -5.0f, -1.0f)), Color(1.0f, 1.0f, 0.8f, 1.0f), 30.0f);
   
    m_pAxis = NEW he::game::Entity();
    game::ModelComponent* pAxisModelComp(NEW game::ModelComponent());
    pAxisModelComp->setMaterial(CONTENT->loadMaterial("axis.material"));
    pAxisModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("axis.binobj", "M_Axis", pAxisModelComp->getMaterial().getCompatibleVertexLayout()));
    m_pAxis->addComponent(pAxisModelComp);

    m_pScene = NEW he::game::Entity();
    game::ModelComponent* pSceneModelComp(NEW game::ModelComponent());
    pSceneModelComp->setMaterial(CONTENT->loadMaterial("testScene.material"));
    pSceneModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("testScene.binobj", "M_Ground", pSceneModelComp->getMaterial().getCompatibleVertexLayout()));
    pSceneModelComp->setVisible(true);
    m_pScene->addComponent(pSceneModelComp);
    game::StaticPhysicsComponent* pScenePxComp(NEW game::StaticPhysicsComponent());
    m_pScene->addComponent(pScenePxComp);
    const auto& pSceneCVmeshes(CONTENT->loadPhysicsConvex("testScene.pxcv"));
    std::for_each(pSceneCVmeshes.cbegin(), pSceneCVmeshes.cend(), [&](const px::PhysicsConvexMesh::pointer& pMesh)
    {
        he::px::PhysicsConvexShape pShape(pMesh);
        pScenePxComp->addShape(&pShape, PHYSICS->getDriveableMaterial(DM_Concrete));
    });
    const auto& pSceneCCmeshes(CONTENT->loadPhysicsConcave("testScene.pxcc"));
    std::for_each(pSceneCCmeshes.cbegin(), pSceneCCmeshes.cend(), [&](const px::PhysicsConcaveMesh::pointer& pMesh)
    {
        he::px::PhysicsConcaveShape pShape(pMesh);
        pScenePxComp->addShape(&pShape, PHYSICS->getDriveableMaterial(DM_Tarmac));
    });

    m_pSky = NEW he::game::Entity();
    game::ModelComponent* pSkyModelComp(NEW game::ModelComponent());
    pSkyModelComp->setMaterial(CONTENT->loadMaterial("sky.material"));
    pSkyModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("skydome.binobj", "M_Sky", pSkyModelComp->getMaterial().getCompatibleVertexLayout()));
    pSkyModelComp->setLocalTransform(mat44::createScale(vec3(500, 100, 500)));
    pSkyModelComp->setCastsShadow(false);
    m_pSky->addComponent(pSkyModelComp);
        
    m_TestImage = CONTENT->asyncLoadTexture("v8_vantage_color.png");

    m_pFont = CONTENT->loadFont("MODES.ttf", 12);

    m_pTestButton = NEW gui::Button(gui::Button::TYPE_NORMAL, vec2(1000, 600), vec2(60,20));
    m_pTestButton->setText("Play 2D", 12);
    m_pTestButton2 = NEW gui::Button(gui::Button::TYPE_NORMAL, vec2(1000,630), vec2(60,20));
    m_pTestButton2->setText("Stop", 12);
    m_pTestButton3 = NEW gui::Button(gui::Button::TYPE_NORMAL, vec2(1000, 570), vec2(60,20));
    m_pTestButton3->setText("Play 3D", 12);

    m_pTextBox = NEW gui::TextBox(RectF(50,650,200,20), "testing", 10);

    CONSOLE->registerVar<bool>(&m_bTest2, "draw_test");
    CONSOLE->registerVar<std::string>(&m_Test3, "test_string");
    CONSOLE->addMessage("warning test", CMSG_TYPE_WARNING);

    m_pTestSound2D = AUDIO->loadSound2D("../data/audio/goodkat_dnb.wav", true);
    m_pTestSound2D->setLooping(true);
    m_pTestSound2D->setPitch(0.6f);

    m_pTestSound3D = AUDIO->loadSound3D("../data/audio/goodkat_dnb.wav", false);
    m_pTestSound3D->setLooping(true);
    m_pTestSound3D->setMaximumDistance(50.0f);
    m_pTestSound3D->setMinimumDistance(10.0f);

    m_pTestObject = NEW TestObject();

    m_pTestGrid = NEW he::tools::Grid(he::vec3(0,0,0), 100, 1.0f);
    m_pTestGrid->setColor(Color(0.6f,0.6f,0.6f), Color(1.0f,1.0f,1.0f));

    GRAPHICS->initPicking();

    //////////////////////////////////////////////////////////////////////////
    ///                            CAMERA'S                                ///
    //////////////////////////////////////////////////////////////////////////
    m_pFlyCamera = NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
    m_pFlyCamera->lookAt(vec3(-5, 5, -4), vec3(0, 0, 0), vec3(0, 1, 0));
    m_pFlyCamera->setLens(16.0f/9.0f,piOverFour,10.0f,400.0f);

    m_pFollowCamera = NEW FollowCamera();
    m_pFollowCamera->setLens(16.0f/9.0f,piOverFour,10.0f, 200.0f);
    m_pFollowCamera->setFollowObject(m_pTestObject);
    m_pFollowCamera->setLocalLook(normalize(vec3(0, 0.58f, -1.0f)));
    m_pFollowCamera->setDistance(15);

    //m_pCurrentCamera = m_pFollowCamera;
    //GAME->setActiveCamera(m_pFollowCamera);
    m_pCurrentCamera = m_pFlyCamera;
    CAMERAMANAGER->addCamera("lol",m_pFlyCamera);
    CAMERAMANAGER->setActiveCamera("lol");

    //m_pFxEditorBinding->init();
    //GUI->setBlending(true);

}
void MainGame::tick(float dTime)
{
    using namespace he;

    PROFILER_BEGIN("MainGame::tick");

    /*if (m_pCurrentCamera == m_pFlyCamera)
        m_pFlyCamera->tick(dTime);
    else
        m_pFollowCamera->tick(dTime);*/

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Escape))
        HAPPYENGINE->quit();

    m_pTestObject->tick(dTime);

    //m_pTestSound2D->setPosition(m_pTestObject->getWorldMatrix().getTranslation());

    AUDIO->setListenerPos(m_pCurrentCamera->getPosition());
    AUDIO->setListenerOrientation(m_pCurrentCamera->getLook(), m_pCurrentCamera->getUp());

    m_pCarLight->setPosition(m_pTestObject->getWorldMatrix().getTranslation() + vec3(0, 2, 0));
    
    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Space))
    {
        game::Entity* pBullet(NEW game::Entity());
        
        pBullet->setWorldMatrix(mat44::createTranslation(m_pCurrentCamera->getPosition()));

        game::DynamicPhysicsComponent* pPhysicsComponent(NEW game::DynamicPhysicsComponent());
        pBullet->addComponent(pPhysicsComponent);
        px::PhysicsBoxShape boxShape(vec3(2, 2, 2));
        pPhysicsComponent->addShape(&boxShape, PHYSICS->getDriveableMaterial(DM_Metal), 5);
        pPhysicsComponent->getDynamicActor()->setVelocity(m_pCurrentCamera->getLook() * 20);

        game::ModelComponent* pBulletModelComp(NEW game::ModelComponent());
        pBulletModelComp->setMaterial(CONTENT->loadMaterial("bullet.material"));
        pBulletModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("cube.binobj", "M_Cube", pBulletModelComp->getMaterial().getCompatibleVertexLayout()));
        pBullet->addComponent(pBulletModelComp);


        m_Bullets.push_back(pBullet);
        std::cout << m_Bullets.size() << "\n";
    }

    he::game::Game::tick(dTime); //tick all components

    //m_pSpotLight->setPosition(m_pCamera->getPosition());
    //m_pSpotLight->setDirection(-he::normalize(m_pCamera->getLook()));

    m_pTestButton->tick();
    m_pTestButton2->tick();
    m_pTestButton3->tick();

    if (m_pTestButton->isClicked())
    {
        m_pTestSound2D->play(true);
    }
    else if (m_pTestButton2->isClicked())
    {
        m_pTestSound2D->stop();
        m_pTestSound3D->stop();
    }
    else if (m_pTestButton3->isClicked())
    {
        m_pTestSound3D->play(true);
    }

    m_pTextBox->tick();

    if (CONTROLS->getMouse()->isButtonPressed(he::io::MouseButton_Left))
    {
        he::uint id(GRAPHICS->pick(CONTROLS->getMouse()->getPosition()));

        /*if (id != UINT_MAX)
        {
            m_PickPos = GRAPHICS->getDrawList()[id]->getWorldMatrix().getTranslation();
        }
        else*/
            m_PickPos = vec3(id,id,id);
    }

    m_pFPSGraph->tick(dTime, 0.5f);

    PROFILER_END("MainGame::tick");
}
void MainGame::drawGui()
{
    using namespace he;
    using namespace gfx;

    PROFILER_BEGIN("MainGame::draw");
    // 2D test stuff
    /*m_pTestButton->draw();
    m_pTestButton2->draw();
    m_pTestButton3->draw();*/
        
    /*m_pTextBox->draw();

    if (m_bTest2)
    {
        GUI->setColor(1.0f,0.5f,0.0f);
        GUI->setFontVerticalAlignment(Font::VAlignment_Center);
        GUI->setFontHorizontalAlignment(Font::HAlignment_Center);

        GUI->drawString(m_Test3, m_pFont, RectF(0,0,(float)GRAPHICS->getScreenWidth(),(float)GRAPHICS->getScreenHeight()));
    }*/

    /*GUI->setColor(1.0f,1.0f,1.0f);

    std::stringstream stream;
    stream << "2D: " << m_pTestSound2D->getPlayTime() << " / " << m_pTestSound2D->getLength();

    GUI->drawText(gui::Text(stream.str()), vec2(1050,570));

    stream.str("");

    stream << "3D: " << m_pTestSound3D->getPlayTime() << " / " << m_pTestSound3D->getLength();

    GUI->drawText(gui::Text(stream.str()), vec2(1050,590));*/

    /*stream.str("");

    stream << "pos: " << m_PickPos.x << " " << m_PickPos.y << " " << m_PickPos.z;*/

    //GUI->drawText(gui::Text(stream.str()), vec2(1050,610));

    //if (m_Poly.getPolygon().getVertexCount() > 2)
    //{
    //    GUI->setAntiAliasing(true);
    //    GUI->setColor(1.0f,0.0f,1.0f);
    //    GUI->fillShape2D(m_Poly);
    //    /*GUI->setColor(1.0f,1.0f,1.0f);
    //    GUI->drawShape2D(m_Poly);*/
    //}

    //GUI->setAntiAliasing(true);
    //GUI->setColor(1.0f,0.0f,1.0f);
    //GUI->fillShape2D(gui::RoundedRectangle2D(vec2(500,500), vec2(250,100), 50));

    m_pFPSGraph->draw();

    ///* DRAW 3D & 2D */
    //GRAPHICS->clearAll();

    //GRAPHICS->begin(m_pCurrentCamera);
    //GRAPHICS->end();

    // TODO: implement into drawmanager/GRAPHICS
    HE3D->begin(CAMERAMANAGER->getActiveCamera());
    m_pTestGrid->draw();
    //HE3D->drawBillboard(m_TestImage, vec3(0,5.0f,0));
    HE3D->end();
    PROFILER_END("MainGame::draw");
}

} //end namespace