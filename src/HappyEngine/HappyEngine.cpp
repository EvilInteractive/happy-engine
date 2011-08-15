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

#include "HappyEngine.h"

#include <iostream>

#if _DEBUG
    #undef new
#endif
#include "boost/thread.hpp"
#if _DEBUG
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "Assert.h"
#include "ExternalError.h"
#include "IL/il.h"
#include "IL/ilu.h"

namespace happyengine {

HappyEngine::pointer HappyEngine::s_pHappyEngine = boost::shared_ptr<HappyEngine>(new HappyEngine());

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false), m_Loaded(false), 
                            m_pGraphicsEngine(nullptr), m_pControlsManager(nullptr)
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
    cleanup();
    std::cout << "\n--Thank you for using HappyEngine--\n";
}
void HappyEngine::cleanup()
{   
    //dispose/delete all sub engines here
    delete m_pGraphicsEngine;
    m_pGraphicsEngine = nullptr;
    delete m_pControlsManager;
    m_pControlsManager = nullptr;

    SDL_Quit();
}
void HappyEngine::initSubEngines()
{
    ilInit();
    iluInit();
    iluSetLanguage(ILU_ENGLISH);
    //init all sub engines here
    m_pGraphicsEngine = new graphics::GraphicsEngine();
    m_pControlsManager = new io::ControlsManager();
}

void HappyEngine::start(IGame* pGame)
{
    cleanup();

    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    cout << "os: " << SDL_GetPlatform() << "\n\n";

    m_pGame = pGame;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Init other stuff
    initSubEngines();

    //Init Game
    pGame->init();
    
    //Run update loop
    boost::thread updateThread(boost::bind(&HappyEngine::updateLoop, this));

    //Run draw loop
    drawLoop();
    
    updateThread.join();
}
void HappyEngine::updateLoop()
{
    Uint32 prevTicks = SDL_GetTicks();
    while (m_Quit == false)
    {
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;

        if (m_Loaded)
            m_pGame->tick(dTime);

        SDL_Delay(12);
    }
}
void HappyEngine::drawLoop()
{
    m_pGraphicsEngine->init();
    m_pGame->load();

    m_Loaded = true;
    Uint32 prevTicks = SDL_GetTicks();
    SDL_Event event;
    while (m_Quit == false)
    {
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;

        while (SDL_PollEvent(&event)) //Events are window related ==> need to be in the same thread
        {
            switch (event.type)
            {
                case SDL_QUIT: m_Quit = true; break;
            }
        }            
        m_pControlsManager->tick(); 

        m_pGame->draw(dTime);
        
        m_pGraphicsEngine->present();
    }
}

HappyEngine::pointer HappyEngine::getPointer()
{
    return s_pHappyEngine;
}


//SubEngines
graphics::GraphicsEngine* HappyEngine::getGraphicsEngine() const
{
    return m_pGraphicsEngine;
}
const io::ControlsManager* HappyEngine::getControls() const
{
    return m_pControlsManager;
}

} //end namespace
