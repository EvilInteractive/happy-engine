//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "HappyPCH.h" 

#include "HappyEngine.h"
#include "HappyNew.h"

#include <iostream>

#include "HeAssert.h"
#include "ExternalError.h"
#include "IL/il.h"
#include "IL/ilu.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "PhysicsEngine.h"
#include "ContentManager.h"
#include "NetworkManager.h"
#include "Happy2DRenderer.h"
#include "Game.h"
#include "CameraManager.h"
#include "Console.h"
#include "SoundEngine.h"
#include "SimpleForward3DRenderer.h"
#include "fx/FxManager.h"

#ifdef HE_ENABLE_QT
#pragma warning(disable:4127)
#include <QtGui/QApplication>
#pragma warning(default:4127)
#include "HappyQtWidget.h"
#endif

namespace he {

HappyEngine* HappyEngine::s_pHappyEngine = nullptr;

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false),
                            m_pGraphicsEngine(nullptr), m_pControlsManager(nullptr),
                            m_pPhysicsEngine(nullptr), m_pContentManager(nullptr),
                            m_pNetworkManager(nullptr), m_p2DRenderer(nullptr),
                            m_pConsole(nullptr), m_pSoundEngine(nullptr), m_p3DRenderer(nullptr), m_SubEngines(0),
                            m_pCameraManager(nullptr), m_bShowProfiler(false), m_pLoadingScreen(nullptr), m_bGameLoading(true)
{
}
HappyEngine::~HappyEngine()
{
    tools::Profiler::dispose();
}

void HappyEngine::quit()
{
    m_Quit = true;
}
void HappyEngine::dispose()
{
    HE_INFO("");
    HE_INFO("--Thank you for using HappyEngine--");
    HAPPYENGINE->cleanup();
    delete s_pHappyEngine;
    s_pHappyEngine = nullptr;
}
void HappyEngine::cleanup()
{  
    m_AudioThread.join(); // wait for audiothread to finish

    //dispose/delete all sub engines here
    delete m_pContentManager;
    m_pContentManager = nullptr;
    delete m_p2DRenderer;
    m_p2DRenderer = nullptr;
    delete m_pSoundEngine;
    m_pSoundEngine = nullptr;
    delete m_pControlsManager;
    m_pControlsManager = nullptr;
    delete m_pPhysicsEngine;
    m_pPhysicsEngine = nullptr;
    delete m_pNetworkManager;
    m_pNetworkManager = nullptr;
    delete m_p3DRenderer;
    m_p3DRenderer = nullptr;
    delete m_pFxManager;
    m_pFxManager = nullptr;
    delete m_pCameraManager;
    m_pCameraManager = nullptr;


    delete m_pGraphicsEngine;
    m_pGraphicsEngine = nullptr;

	delete m_pLoadingScreen;

    if (m_SubEngines & SubEngine_Graphics)
    {
        SDL_Quit();
    }

    delete m_pConsole;
    m_pConsole = nullptr;
}
void HappyEngine::init(int subengines)
{
    if (s_pHappyEngine == nullptr)
        s_pHappyEngine = NEW HappyEngine();
    GL::init();
    HAPPYENGINE->initSubEngines(subengines);
}
void HappyEngine::initSubEngines(int subengines = SubEngine_All)
{
    m_SubEngines |= subengines;

    m_pConsole = NEW tools::Console();

    if (subengines & SubEngine_Graphics)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        m_pGraphicsEngine = NEW gfx::GraphicsEngine();
        m_p3DRenderer = NEW gfx::SimpleForward3DRenderer();
        m_pCameraManager = NEW game::CameraManager();
    }

    if (subengines & SubEngine_Content)
    {
        ilInit();
        iluInit();
        iluSetLanguage(ILU_ENGLISH);
        m_pContentManager = NEW ct::ContentManager();
    }
    
    if (subengines & SubEngine_Controls)
    {
        m_pControlsManager = NEW io::ControlsManager((subengines & SubEngine_Qt) == SubEngine_Qt);
    }

    if (subengines & SubEngine_Physics)
    {
        m_pPhysicsEngine = NEW px::PhysicsEngine();
    }

    if (subengines & SubEngine_Networking)
    {
        m_pNetworkManager = NEW net::NetworkManager();
    }

    if (subengines & SubEngine_2DRenderer)
    {
        m_p2DRenderer = NEW gfx::Happy2DRenderer();
    }

    if (subengines & SubEngine_Audio)
    {
        m_pSoundEngine = NEW sfx::SoundEngine();
        m_pSoundEngine->initialize();
    }

    m_pConsole->load();

    m_pFxManager = NEW gfx::FxManager();
}

void HappyEngine::start(game::Game* pGame)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    cout << "os: " << SDL_GetPlatform() << "\n\n";

    m_pGame = pGame;

    CONSOLE->registerVar(&m_bShowProfiler, "s_profiler");

    //Init Game
    pGame->init();
    
    //load stuff
    if (m_SubEngines & SubEngine_Graphics)
    {
        m_pGraphicsEngine->init(false);
        m_p3DRenderer->init();
        m_pCameraManager->init();
    }

    if (m_SubEngines & SubEngine_2DRenderer) m_p2DRenderer->init();

	m_pLoadingScreen = NEW tools::LoadingScreen();

    m_pGame->load();

    m_AudioThread = boost::thread(&HappyEngine::audioLoop, this);

    //boost::timer t;
    m_PrevTime = boost::chrono::high_resolution_clock::now();

    while (m_Quit == false)
    {
        loop();
    }   
}
#ifdef HE_ENABLE_QT
void HappyEngine::start(gfx::HappyQtWidget* pWidget)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************   Qt   ****************  \n";
    cout << "*************  HappyEngine :3  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    m_SubEngines |= SubEngine_Qt;    

    m_pGame = pWidget;
    m_pCameraManager->init();
    m_pGame->load();
    m_pQtWidget = pWidget;
    
    m_AudioThread = boost::thread(&HappyEngine::audioLoop, this);

    m_qtLoopTimer.setSingleShot(true); //as fast as possible
    connect(&m_qtLoopTimer, SIGNAL(timeout()), this, SLOT(loop()));
    m_qtLoopTimer.start(0);

    QApplication::exec();
    m_Quit = true;
}
#endif
void HappyEngine::loop()
{
    PROFILER_BEGIN("Main Loop");
    boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - m_PrevTime);
    m_PrevTime = boost::chrono::high_resolution_clock::now();
    float dTime(elapsedTime.count() / static_cast<float>(boost::nano::den));
    updateLoop(dTime);
    if (m_SubEngines & SubEngine_Graphics)
    {
        drawLoop();
    }
    PROFILER_END();

#ifdef HE_ENABLE_QT
    if ((m_SubEngines & SubEngine_Qt) && m_Quit == false)
        m_qtLoopTimer.start(0);
#endif
}
void HappyEngine::updateLoop(float dTime)
{
    PROFILER_BEGIN("Update Loop");
    if ((m_SubEngines & SubEngine_Qt) != SubEngine_Qt)
    {
        SDL_Event event;
        m_SDLEvents.clear();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: m_Quit = true; break;
            }

            // needed for checking events in other places in program
            m_SDLEvents.push_back(event);
        }
    }
    if (m_SubEngines & SubEngine_Controls)
        m_pControlsManager->tick();
    if (m_SubEngines & SubEngine_Content)
    {
        m_pContentManager->tick(dTime);
        m_pContentManager->glThreadInvoke();
    }
    if (m_SubEngines & SubEngine_Physics)
        m_pPhysicsEngine->tick(dTime);

    m_pFxManager->tick(dTime);

    m_pGame->tick(dTime);

    CONSOLE->tick();

	if (CONTENT->isLoading() == false && m_bGameLoading == true)
        m_bGameLoading = false;

    PROFILER_END();
}
void HappyEngine::drawLoop()
{
    PROFILER_BEGIN("Draw Loop");
    // display 3D scene
    GRAPHICS->drawScene();

	// draw 2D stuff
	if (m_bGameLoading)
        drawLoadingScreen();
	else
		m_pGame->drawGui();

    // draw profiler if needed
    if (m_bShowProfiler)
        PROFILER->draw();

    // draw console
    CONSOLE->draw();

	// display 2D
	GUI->draw();    
 
#ifdef HE_ENABLE_QT
    if (m_SubEngines & SubEngine_Qt)
        m_pQtWidget->present();
    else 
#endif
    if (m_SubEngines & SubEngine_Graphics)
        m_pGraphicsEngine->present();
    PROFILER_END();
}

HappyEngine* HappyEngine::getPointer()
{
    return s_pHappyEngine;
}

const std::vector<SDL_Event>& HappyEngine::getSDLEvents() const
{
    return m_SDLEvents;
}

void HappyEngine::audioLoop()
{
    boost::posix_time::milliseconds waitTime = boost::posix_time::milliseconds(1);
    boost::chrono::high_resolution_clock::time_point prevTime(boost::chrono::high_resolution_clock::now());
    float dTime(0);

    while (m_Quit == false)
    {
        boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - prevTime);
        prevTime = boost::chrono::high_resolution_clock::now();
        dTime += (elapsedTime.count() / static_cast<float>(boost::nano::den));

        if (dTime >= (1 / 30.0f))
        {
            m_pSoundEngine->tick(dTime);
            dTime = 0;
        }
        else
            boost::this_thread::sleep(waitTime);
    }
}

void HappyEngine::drawLoadingScreen()
{
    m_pLoadingScreen->tick();

    GUI->setDepth(1);

    m_pLoadingScreen->draw();
}

//SubEngines
gfx::GraphicsEngine* HappyEngine::getGraphicsEngine() const
{
    return m_pGraphicsEngine;
}
const io::ControlsManager* HappyEngine::getControls() const
{
    return m_pControlsManager;
}
px::PhysicsEngine* HappyEngine::getPhysics() const
{
    return m_pPhysicsEngine;
}
ct::ContentManager* HappyEngine::getContentManager() const
{
    return m_pContentManager;
}
net::NetworkManager* HappyEngine::getNetworkManager() const
{
    return m_pNetworkManager;
}
gfx::Happy2DRenderer* HappyEngine::get2DRenderer() const
{
    return m_p2DRenderer;
}
tools::Console* HappyEngine::getConsole() const
{
    return m_pConsole;
}

sfx::SoundEngine* HappyEngine::getSoundEngine() const
{
    return m_pSoundEngine;
}

game::Game* HappyEngine::getGame() const
{
    return m_pGame;
}

gfx::SimpleForward3DRenderer* HappyEngine::get3DRenderer() const
{
    return m_p3DRenderer;
}

gfx::FxManager* HappyEngine::getFxManager() const
{
    return m_pFxManager;
}

game::CameraManager* HappyEngine::getCameraManager() const
{
    return m_pCameraManager;
}

} //end namespace
