//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#include "IL/il.h"
#include "IL/ilu.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "PhysicsEngine.h"
#include "ContentManager.h"
#include "NetworkManager.h"
#include "Renderer2D.h"
#include "Game.h"
#include "Console.h"
#include "SoundEngine.h"
#include "StaticDataManager.h"
#include "Window.h"

namespace he {

HappyEngine* HappyEngine::s_pHappyEngine = nullptr;

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false),
                            m_pGraphicsEngine(nullptr), m_pControlsManager(nullptr),
                            m_pPhysicsEngine(nullptr), m_pContentManager(nullptr),
                            m_pNetworkManager(nullptr),
                            m_pConsole(nullptr), m_pSoundEngine(nullptr), m_SubEngines(0),
                            m_bShowProfiler(false), m_bGameLoading(true),
                            m_RootDir("./")
{
}
HappyEngine::~HappyEngine()
{
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

    StaticDataManager::destroy();
    delete HAPPYENGINE->m_pPhysicsEngine;

    delete s_pHappyEngine;
    s_pHappyEngine = nullptr;
}
void HappyEngine::cleanup()
{  
    tools::Profiler::dispose();

    m_AudioThread.join(); // wait for audiothread to finish

    //dispose/delete all sub engines here
    m_pGame = nullptr;
    delete m_pGraphicsEngine;
    m_pGraphicsEngine = nullptr;
    delete m_pContentManager;
    m_pContentManager = nullptr;
    delete m_pSoundEngine;
    m_pSoundEngine = nullptr;
    delete m_pControlsManager;
    m_pControlsManager = nullptr;
    delete m_pNetworkManager;
    m_pNetworkManager = nullptr;

    delete m_pConsole;
    m_pConsole = nullptr;
}
void HappyEngine::init(int subengines)
{
    StaticDataManager::init();
    if (s_pHappyEngine == nullptr)
        s_pHappyEngine = NEW HappyEngine();
    HAPPYENGINE->initSubEngines(subengines);
}
void HappyEngine::initSubEngines(int subengines = SubEngine_All)
{
    m_SubEngines |= subengines;


    if (subengines & SubEngine_Graphics)
    {
        m_pGraphicsEngine = NEW gfx::GraphicsEngine();
        m_pConsole = NEW tools::Console();
    }

    if (subengines & SubEngine_Content)
    {
        ilInit();
        iluInit();
        iluSetLanguage(ILU_ENGLISH);
        ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
        ilEnable(IL_ORIGIN_SET);
        ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);
        m_pContentManager = NEW ct::ContentManager();
    }
    else
    {
        m_bGameLoading = false;
    }
    
    if (subengines & SubEngine_Controls)
    {
        m_pControlsManager = NEW io::ControlsManager();
    }

    if (subengines & SubEngine_Physics)
    {
        m_pPhysicsEngine = NEW px::PhysicsEngine();
    }

    if (subengines & SubEngine_Networking)
    {
        m_pNetworkManager = NEW net::NetworkManager();
    }

    if (subengines & SubEngine_Audio)
    {
        m_pSoundEngine = NEW sfx::SoundEngine();
        m_pSoundEngine->initialize();
    }
}

void HappyEngine::start(ge::Game* pGame)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    //cout << "os: " << SDL_GetPlatform() << "\n\n";

    int sse(0x01 << 25);
    int sse2(0x01 << 26);
    int sse3(0x01);
    int sse4(0x01 << 19);
    __asm
    {
        // get information to see if it supports XMM register
        mov eax, 01h 
        cpuid
        and sse, edx
        and sse2, edx
        and sse3, ecx
        and sse4, ecx
    } 
    HE_INFO("Supported XMM: %s,%s,%s,%s", sse?"SSE":"", sse2?"SSE2":"", sse3?"SSE3":"", sse4?"SSE4":"");

    m_pGame = pGame;
  
    //Init Game
    pGame->init();

    //load stuff
    if (m_SubEngines & SubEngine_Graphics)
    {
        m_pGraphicsEngine->init();
        
        m_pConsole->load();
        CONSOLE->registerVar(&m_bShowProfiler, "s_profiler");

        PROFILER->load();
    }


    //if (m_SubEngines & SubEngine_2DRenderer) m_p2DRenderer->init();


    m_pGame->load();
    
    if (m_SubEngines & SubEngine_Audio)
    {
        m_AudioThread = boost::thread(&HappyEngine::audioLoop, this);
    }

    //boost::timer t;
    m_PrevTime = boost::chrono::high_resolution_clock::now();

    while (m_Quit == false)
    {
        loop();
    }   
}
void HappyEngine::loop()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - m_PrevTime);
    m_PrevTime = boost::chrono::high_resolution_clock::now();

    float dTime(elapsedTime.count() / static_cast<float>(boost::nano::den));

    updateLoop(dTime);

    if (m_SubEngines & SubEngine_Graphics)
    {
        drawLoop();
    }
    else
    {
        boost::this_thread::sleep(boost::posix_time::millisec(5));
    }
}
void HappyEngine::updateLoop(float dTime)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    PROFILER_BEGIN("SFML poll events");
    const std::vector<gfx::Window*>& windows(GRAPHICS->getAllWindows());
    std::for_each(windows.cbegin(), windows.cend(), [&dTime](gfx::Window* window)
    {
        window->doEvents(dTime);
    });
    PROFILER_END();

    if (m_SubEngines & SubEngine_Controls)
        m_pControlsManager->tick();

    if (m_SubEngines & SubEngine_Networking)
        m_pNetworkManager->tick(dTime);

    if (m_SubEngines & SubEngine_Content)
    {
        m_pContentManager->tick(dTime);
        m_pContentManager->glThreadInvoke();
    }

    if (m_SubEngines & SubEngine_Physics)
        m_pPhysicsEngine->tick(dTime);

    m_pGame->tick(dTime);

    if (m_SubEngines & SubEngine_Graphics)
    {
        m_pGraphicsEngine->tick(dTime);
        CONSOLE->tick();
        if (CONTENT->isLoading() == false && m_bGameLoading == true)
            m_bGameLoading = false;
    }

}
void HappyEngine::drawLoop()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    // render everything
    GRAPHICS->draw();
}

HappyEngine* HappyEngine::getPointer()
{
    return s_pHappyEngine;
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

        if (dTime >= (1 / 60.0f))
        {
            m_pSoundEngine->tick(dTime);
            dTime = 0;
        }
        else
            boost::this_thread::sleep(waitTime);
    }
}

//SubEngines
gfx::GraphicsEngine* HappyEngine::getGraphicsEngine() const
{
    return m_pGraphicsEngine;
}
io::ControlsManager* HappyEngine::getControls() const
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
tools::Console* HappyEngine::getConsole() const
{
    return m_pConsole;
}

sfx::SoundEngine* HappyEngine::getSoundEngine() const
{
    return m_pSoundEngine;
}

ge::Game* HappyEngine::getGame() const
{
    return m_pGame;
}

void HappyEngine::setRootDir( const Path& rootDir )
{
    m_RootDir = rootDir;
}

const Path& HappyEngine::getRootDir() const
{
    return m_RootDir;
}

} //end namespace
