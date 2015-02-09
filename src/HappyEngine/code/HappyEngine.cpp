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
#include "WindowSDL.h"
#include "Gui.h"
#include "Sprite.h"
#include "PluginLoader.h"
#include "Timer.h"
#include "IKeyboard.h"
#include "IMouse.h"
#include "MemoryManager.h"

#include <SDL2/SDL.h>
#include <locale.h>

namespace he {

HappyEngine* gEngine(nullptr);

HappyEngine::HappyEngine(): m_Game(nullptr), m_Quit(false),
                            m_GraphicsEngine(nullptr), m_ControlsManager(nullptr),
                            m_PhysicsEngine(nullptr), m_ContentManager(nullptr),
                            m_NetworkManager(nullptr),
                            m_Console(nullptr), m_SoundEngine(nullptr), m_SubEngines(0),
                            m_ShowProfiler(false), m_GameLoading(true),
                            m_RootDir("./"), m_Gui(nullptr), m_PluginLoader(nullptr)
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
    he::Path::destroy();
    HEDelete(gEngine);
    gEngine = nullptr;

    ::setlocale(LC_ALL, nullptr);
    if (gMemMan)
    {
        gMemMan->~MemoryManager();
        free(gMemMan);
        gMemMan = nullptr;
    }
}
void HappyEngine::cleanup()
{  
    HE_ASSERT(m_Quit, "Please call HAPPYENGINE->Quit() first before disposing");
    m_Quit = true;

    tools::Profiler::dispose();

    m_AudioThread.join(); // wait for audiothread to finish
    
    HEDelete(m_Console);
    m_Console = nullptr;

    m_Game = nullptr;

    HEDelete(m_Gui);
    m_Gui = nullptr;

    if (m_ContentManager != nullptr)
        m_ContentManager->destroy();

    if (m_GraphicsEngine != nullptr)
        m_GraphicsEngine->destroy();

    //dispose/HEDelete(all) sub engines here
    
    HEDelete(m_PluginLoader);
    m_PluginLoader = nullptr;
    HEDelete(m_ContentManager);
    m_ContentManager = nullptr;
    HEDelete(m_SoundEngine);
    m_SoundEngine = nullptr;
    HEDelete(m_ControlsManager);
    m_ControlsManager = nullptr;
    HEDelete(m_NetworkManager);
    m_NetworkManager = nullptr;
    HEDelete(m_PhysicsEngine);
    m_PhysicsEngine = nullptr;
    HEDelete(m_GraphicsEngine);
    m_GraphicsEngine = nullptr;
}
void HappyEngine::init(const int argc, const char* const * const argv, const int subengines)
{
    ::setlocale(LC_ALL, "en_US.UTF-8");
    HE_ASSERT(argc > 0, "There must be at least one argument in the argv argument list!");
    HE_ASSERT(!gMemMan, "Memory Manager is already initialized!");
    gMemMan = new(malloc(sizeof(MemoryManager))) MemoryManager();
    HE_ASSERT(!gEngine, "Happy Engine is already initialized!");
    gEngine = HENew(HappyEngine)();
    Path::init(argc, argv);
    StaticDataManager::init();
    ThreadTicketManager* const treadTicketMan(ThreadTicketManager::getInstance());
    treadTicketMan->registerTicket(eThreadTicket_Main, "Main");
    treadTicketMan->registerTicket(eThreadTicket_Content, "Content");
    CLAIM_THREAD(eThreadTicket_Main);
    HE_INFO("Bin path: %s", Path::getBinPath().str().c_str());
    HE_INFO("Data path: %s", Path::getDataPath().str().c_str());
    HE_INFO("User path: %s", Path::getUserDir().str().c_str());
    HAPPYENGINE->initSubEngines(subengines);
}
void HappyEngine::initSubEngines(int subengines = SubEngine_All)
{
    m_SubEngines |= subengines;

    if (subengines & SubEngine_Graphics)
    {
        m_GraphicsEngine = HENew(gfx::GraphicsEngine)();
        m_Console = HENew(tools::Console)();

        ilInit();
        iluInit();
        iluSetLanguage(ILU_ENGLISH);
        ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
        ilEnable(IL_ORIGIN_SET);
        ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

        m_Gui = HENew(gui::Gui)();
    }

    if (subengines & SubEngine_Controls)
    {
        m_ControlsManager = HENew(io::ControlsManager)();
    }

    if (subengines & (SubEngine_Graphics | SubEngine_Physics | SubEngine_Audio))
    {
        m_ContentManager = HENew(ct::ContentManager)();
    }
    
    if (subengines & SubEngine_Physics)
    {
        m_PhysicsEngine = HENew(px::PhysicsEngine)();
    }

    if (subengines & SubEngine_Networking)
    {
        m_NetworkManager = HENew(net::NetworkManager)();
    }

    if (subengines & SubEngine_Audio)
    {
        m_SoundEngine = HENew(sfx::SoundEngine)();
    }

    m_PluginLoader = HENew(pl::PluginLoader)();
}

void HappyEngine::start(ge::Game* game, const bool managed, he::gfx::Window* sharedContext)
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

#if defined(_MSC_VER) && ARCH_32
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

    HE_ASSERT((m_SubEngines & SubEngine_Windowing) == 0 || sharedContext == 0 || sharedContext->getType() == HEFS::strSDLWindow, "Remove subengine windowing when supplying a shared context that is not an SDL window!");
    
    // Load stuff
    if (m_SubEngines & SubEngine_Graphics)
    {
        m_GraphicsEngine->init((m_SubEngines & SubEngine_Windowing) != 0, sharedContext);
        m_ContentManager->initialize();
        m_SoundEngine->initialize();

        m_Console->load();
#ifdef ENABLE_PROFILING
        PROFILER->load();
#endif
    }  
    
    if (m_SubEngines & SubEngine_Audio)
    {
        m_AudioThread.startThread(std::bind(&HappyEngine::audioLoop, this), "Audio Thread");
    }

    m_PrevTime = std::chrono::high_resolution_clock::now();
    if (managed)
    {
        m_Game->init();
        while (m_Quit == false)
        {
            loop();
        }
        // Destroy Game
        m_Game->destroy();
    }
}
void HappyEngine::loop()
{
    {
        HIERARCHICAL_PROFILE(__HE_FUNCTION__);

        std::chrono::duration<float> elapsedTime(std::chrono::high_resolution_clock::now() - m_PrevTime);
        m_PrevTime = std::chrono::high_resolution_clock::now();

        float dTime(elapsedTime.count());

        updateLoop(dTime);

        if (m_SubEngines & SubEngine_Graphics)
        {
            drawLoop();
        }
        else
        {
            he::Thread::sleep(5);
        }
    }
#ifdef ENABLE_PROFILING
    PROFILER->tick();
#endif
}
void HappyEngine::updateLoop(float dTime)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);

    if (m_SubEngines & SubEngine_Windowing)
    {
        HIERARCHICAL_PROFILE("Window Poll events");
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            he::gfx::WindowFactory* factory(he::gfx::WindowFactory::getInstance());
            const size_t windowCount(factory->getSize());
            for (size_t i(0); i < windowCount; ++i)
            {
                he::gfx::Window* const window(factory->getAt(static_cast<ObjectHandle::IndexType>(i)));
                if (window->getType() == HEFS::strSDLWindow)
                {
                    he::gfx::WindowSDL* sdlWindow = checked_cast<he::gfx::WindowSDL*>(window);
                    if (sdlWindow->getID() == event.window.windowID)
                    {
                        // Window
                        if (event.type == SDL_WINDOWEVENT)
                        {
                            switch (event.window.event)
                            {
                                case SDL_WINDOWEVENT_MOVED:
                                {
                                    window->Moved(event.window.data1, event.window.data2);
                                } break;
                                case SDL_WINDOWEVENT_CLOSE:
                                {
                                    window->Closed();
                                } break;
                                case SDL_WINDOWEVENT_RESIZED:
                                {
                                    window->Resized(event.window.data1, event.window.data2);
                                } break;
                                case SDL_WINDOWEVENT_ENTER:
                                {

                                } break;
                                case SDL_WINDOWEVENT_LEAVE:
                                {

                                } break;
                                case SDL_WINDOWEVENT_FOCUS_GAINED:
                                {
                                    m_GraphicsEngine->setActiveWindow(window);
                                    window->GainedFocus();
                                } break;
                                case SDL_WINDOWEVENT_FOCUS_LOST:
                                {
                                    window->LostFocus();
                                } break;
                            }
                            break;
                        }
                        else if (m_SubEngines & SubEngine_Controls)
                        {
                            io::IMouse* mouse(m_ControlsManager->getMouse(window->getHandle()));
                            io::IKeyboard* keyboard(m_ControlsManager->getKeyboard(window->getHandle()));
                            switch (event.type)
                            {
                                // Mouse
                            case SDL_MOUSEBUTTONDOWN:
                                {
                                    mouse->MouseButtonPressed(static_cast<io::MouseButton>(event.button.button));
                                } break;
                            case SDL_MOUSEBUTTONUP:
                                {
                                    mouse->MouseButtonReleased(static_cast<io::MouseButton>(event.button.button));
                                } break;
                            case SDL_MOUSEMOTION:
                                {
                                    mouse->MouseMoved(vec2(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)));
                                } break;
                            case SDL_MOUSEWHEEL:
                                {
                                    mouse->MouseWheelMoved(event.wheel.y);
                                } break;

                                // Keyboard
                            case SDL_KEYDOWN:
                                {
                                    keyboard->KeyPressed(static_cast<io::Key>(event.key.keysym.scancode));
                                } break;
                            case SDL_KEYUP:
                                {
                                    keyboard->KeyReleased(static_cast<io::Key>(event.key.keysym.scancode));
                                } break;
                            case SDL_TEXTINPUT:
                                {
                                    keyboard->TextEntered(event.text.text);
                                } break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (m_SubEngines & SubEngine_Graphics)
    {
        m_GraphicsEngine->tick(dTime);
        CONSOLE->tick();
        if (m_GameLoading == true && m_ContentManager->isLoading() == false) // TODO: event this
            m_GameLoading = false;
    }

    if (m_SubEngines & SubEngine_Networking)
    {
        m_NetworkManager->tick(dTime);
    }

    if (m_ContentManager != nullptr)
    {
        m_ContentManager->tick(dTime);
        m_ContentManager->glThreadInvoke();
    }

    if (m_Gui != nullptr)
    {
        gui::SpriteCreator* const cr(m_Gui->getSpriteCreator());
        cr->tick(dTime);
        cr->glThreadInvoke();
    }

    if (m_SubEngines & SubEngine_Physics)
    {
        m_PhysicsEngine->tick(dTime);
    }

    m_Game->tick(dTime);

    if (m_SubEngines & SubEngine_Controls)
    {
        m_ControlsManager->tick();
    }
}
void HappyEngine::drawLoop()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    // render everything
    GRAPHICS->draw();
}

void HappyEngine::audioLoop()
{
    Timer timer;

    timer.start();
    while (m_Quit == false)
    {
        const float dTime(timer.getElapsedSecondsF());
        timer.restart();

        m_SoundEngine->tick(dTime);

        he::Thread::sleep(33);
    }
}

} //end namespace
