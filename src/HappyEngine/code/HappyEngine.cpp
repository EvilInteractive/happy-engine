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
#include "StdAfx.h" 

#include "HappyEngine.h"
#include "HappyNew.h"

#include <iostream>

#include "Assert.h"
#include "ExternalError.h"
#include "IL/il.h"
#include "IL/ilu.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "PhysicsEngine.h"
#include "ContentManager.h"
#include "NetworkManager.h"
#include "Happy2DRenderer.h"
#include "Console.h"
#include "SoundEngine.h"

namespace he {

HappyEngine* HappyEngine::s_pHappyEngine = nullptr;

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false),
                            m_pGraphicsEngine(nullptr), m_pControlsManager(nullptr),
                            m_pPhysicsEngine(nullptr), m_pContentManager(nullptr),
                            m_pNetworkManager(nullptr), m_p2DRenderer(nullptr),
							m_pConsole(nullptr), m_pSoundEngine(nullptr)
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
    HAPPYENGINE->cleanup();
    delete s_pHappyEngine;
    s_pHappyEngine = nullptr;
    std::cout << "\n--Thank you for using HappyEngine--\n";
}
void HappyEngine::cleanup()
{  
	m_AudioThread.join(); // wait for audiothread to finish

    //dispose/delete all sub engines here
    delete m_pGraphicsEngine;
    m_pGraphicsEngine = nullptr;
    delete m_pControlsManager;
    m_pControlsManager = nullptr;
    delete m_pContentManager;
    m_pContentManager = nullptr;
    delete m_pPhysicsEngine;
    m_pPhysicsEngine = nullptr;
    delete m_pNetworkManager;
    m_pNetworkManager = nullptr;
	delete m_p2DRenderer;
	m_p2DRenderer = nullptr;
	delete m_pConsole;
	m_pConsole = nullptr;
	delete m_pSoundEngine;
	m_pSoundEngine = nullptr;
    if (m_SubEngines & SubEngine_Graphics)
    {
        SDL_Quit();
    }
}
void HappyEngine::init(int subengines)
{
    if (s_pHappyEngine == nullptr)
        s_pHappyEngine = NEW HappyEngine();
    HAPPYENGINE->initSubEngines(subengines);
}
void HappyEngine::initSubEngines(int subengines = SubEngine_All)
{
    m_SubEngines |= subengines;

    if (subengines & SubEngine_Graphics)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        m_pGraphicsEngine = NEW gfx::GraphicsEngine();
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

	if (subengines & SubEngine_2DRenderer)
    {
		m_p2DRenderer = NEW gfx::Happy2DRenderer();
    }

	if (subengines & SubEngine_Audio)
    {
		m_pSoundEngine = NEW sfx::SoundEngine();
		m_pSoundEngine->initialize();
    }

	m_pConsole = NEW tools::Console();
}

void HappyEngine::start(IGame* pGame)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    cout << "os: " << SDL_GetPlatform() << "\n\n";

    m_pGame = pGame;

    //Init Game
    pGame->init();
    
    //load stuff
    if (m_SubEngines & SubEngine_Graphics) m_pGraphicsEngine->init();
    if (m_SubEngines & SubEngine_2DRenderer) m_p2DRenderer->initialize();
    m_pGame->load();

	m_AudioThread = boost::thread(&HappyEngine::audioLoop, this);

    boost::timer t;
    while (m_Quit == false)
    {
        float dTime(static_cast<float>(t.elapsed()));
        t.restart();

        updateLoop(dTime);
        if (m_SubEngines & SubEngine_Graphics)
            drawLoop(dTime);
    }   
}
void HappyEngine::updateLoop(float dTime)
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
    if (m_SubEngines & SubEngine_Controls) //need to be before the events are updated
        m_pControlsManager->tick();
    if (m_SubEngines & SubEngine_Content)
    {
        m_pContentManager->tick(dTime);
        m_pContentManager->glThreadInvoke();
    }
    if (m_SubEngines & SubEngine_Physics)
        m_pPhysicsEngine->tick(dTime);

    m_pGame->tick(dTime);
}
void HappyEngine::drawLoop(float dTime)
{                
    m_pGame->draw(dTime);
        
    m_pGraphicsEngine->present();
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

	while (m_Quit == false)
	{
		m_pSoundEngine->tick();

		boost::this_thread::sleep(waitTime);
	}
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

} //end namespace
