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
#include "Gui.h"
#include "Sprite.h"

namespace he {

HappyEngine* HappyEngine::s_HappyEngine = nullptr;
Random HappyEngine::s_Random;


HappyEngine::HappyEngine(): m_Game(nullptr), m_Quit(false),
                            m_GraphicsEngine(nullptr), m_ControlsManager(nullptr),
                            m_PhysicsEngine(nullptr), m_ContentManager(nullptr),
                            m_NetworkManager(nullptr),
                            m_Console(nullptr), m_SoundEngine(nullptr), m_SubEngines(0),
                            m_ShowProfiler(false), m_GameLoading(true),
                            m_RootDir("./"), m_Gui(nullptr)
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

    delete s_HappyEngine;
    s_HappyEngine = nullptr;
}
void HappyEngine::cleanup()
{  
    tools::Profiler::dispose();

    m_AudioThread.join(); // wait for audiothread to finish

    if (m_GraphicsEngine != nullptr)
        m_GraphicsEngine->destroy();

    //dispose/delete all sub engines here
    delete m_Gui;
    m_Gui = nullptr;
    delete m_Console;
    m_Console = nullptr;
    delete m_ContentManager;
    m_ContentManager = nullptr;
    m_Game = nullptr;
    delete m_SoundEngine;
    m_SoundEngine = nullptr;
    delete m_ControlsManager;
    m_ControlsManager = nullptr;
    delete m_NetworkManager;
    m_NetworkManager = nullptr;
    delete m_PhysicsEngine;
    m_PhysicsEngine = nullptr;
    
    // Gl context get deleted here - make sure all content is gone
    delete m_GraphicsEngine;
    m_GraphicsEngine = nullptr;
}
void HappyEngine::init(int subengines)
{
    StaticDataManager::init();
    if (s_HappyEngine == nullptr)
        s_HappyEngine = NEW HappyEngine();
    HAPPYENGINE->initSubEngines(subengines);
}
void HappyEngine::initSubEngines(int subengines = SubEngine_All)
{
    m_SubEngines |= subengines;


    if (subengines & SubEngine_Graphics)
    {
        m_GraphicsEngine = NEW gfx::GraphicsEngine();
        m_Console = NEW tools::Console();

        ilInit();
        iluInit();
        iluSetLanguage(ILU_ENGLISH);
        ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
        ilEnable(IL_ORIGIN_SET);
        ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

        m_ControlsManager = NEW io::ControlsManager();

        m_Gui = NEW gui::Gui();
    }

    if (subengines & (SubEngine_Graphics | SubEngine_Physics | SubEngine_Audio))
        m_ContentManager = NEW ct::ContentManager();
    
    if (subengines & SubEngine_Physics)
    {
        m_PhysicsEngine = NEW px::PhysicsEngine();
    }

    if (subengines & SubEngine_Networking)
    {
        m_NetworkManager = NEW net::NetworkManager();
    }

    if (subengines & SubEngine_Audio)
    {
        m_SoundEngine = NEW sfx::SoundEngine();
        m_SoundEngine->initialize();
    }
}

void HappyEngine::start(ge::Game* game)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
#ifdef HE_WINDOWS
    cout << "os: Windows\n\n";
#elif defined(HE_MAC)
    cout << "os: Mac\n\n";
#elif defined(HE_LINUX)
    cout << "os: Linux\n\n";
#endif

#if MSVC && ARCH_32
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
#endif
    m_Game = game;
  
    // Init Game
    game->init();

    // Load stuff
    if (m_SubEngines & SubEngine_Graphics)
    {
        m_GraphicsEngine->init();        
        m_Console->load();

#ifdef ENABLE_PROFILING
        PROFILER->load();
#endif
    }
    
    m_Game->load();
    
    if (m_SubEngines & SubEngine_Audio)
    {
        m_AudioThread = boost::thread(&HappyEngine::audioLoop, this);
    }

    m_PrevTime = boost::chrono::high_resolution_clock::now();
    while (m_Quit == false)
    {
        loop();
    }   

    // Destroy Game
    game->destroy();
}
void HappyEngine::loop()
{
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
#ifdef ENABLE_PROFILING
    PROFILER->tick();
#endif
}
void HappyEngine::updateLoop(float dTime)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    if (m_SubEngines & SubEngine_Graphics)
    {
        m_ControlsManager->tick();

        PROFILER_BEGIN("SFML poll events");
        const he::ObjectList<ObjectHandle>& windows(GRAPHICS->getAllWindows());
        gfx::WindowFactory* windowFactory(gfx::WindowFactory::getInstance());
        std::for_each(windows.cbegin(), windows.cend(), [&dTime,windowFactory](const ObjectHandle& window)
        {
            windowFactory->get(window)->doEvents(dTime);
        });
        PROFILER_END();

        m_GraphicsEngine->tick(dTime);
        CONSOLE->tick();
        if (m_GameLoading == true && CONTENT->isLoading() == false) // TODO: event this
            m_GameLoading = false;
    }

    if (m_SubEngines & SubEngine_Networking)
        m_NetworkManager->tick(dTime);

    if (m_ContentManager != nullptr)
    {
        m_ContentManager->tick(dTime);
        m_ContentManager->glThreadInvoke();
    }

    if (m_Gui != nullptr)
    {
        m_Gui->Sprites->tick(dTime);
        m_Gui->Sprites->glThreadInvoke();
    }

    if (m_SubEngines & SubEngine_Physics)
        m_PhysicsEngine->tick(dTime);

    m_Game->tick(dTime);
}
void HappyEngine::drawLoop()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    // render everything
    GRAPHICS->draw();
}

HappyEngine* HappyEngine::getPointer()
{
    return s_HappyEngine;
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
            m_SoundEngine->tick(dTime);
            dTime = 0;
        }
        else
            boost::this_thread::sleep(waitTime);
    }
}

} //end namespace
