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

#include "boost/thread.hpp"
#include "Assert.h"
#include "ExternalError.h"

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
    //dispose/delete all sub engines here
    delete m_pGraphicsEngine;
    delete m_pControlsManager;

    SDL_Quit();

    std::cout << "--Thank you for using HappyEngine--\n";
}
void HappyEngine::initSubEngines()
{
    //init all sub engines here
    m_pGraphicsEngine = new graphics::GraphicsEngine();
    m_pControlsManager = new io::ControlsManager();
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

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Init other stuff
    initSubEngines();

    //Init Game
    pGame->init();
    
    //Start draw thread
    boost::thread drawThread(boost::bind(&HappyEngine::drawThread, this));

    //Run main update loop
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
    
    drawThread.join();

    quit();
}
void HappyEngine::drawThread()
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
