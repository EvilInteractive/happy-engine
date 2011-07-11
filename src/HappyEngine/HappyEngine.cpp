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

namespace happyengine {

HappyEngine::pointer HappyEngine::s_pHappyEngine = boost::shared_ptr<HappyEngine>(new HappyEngine());

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false)
{
}
HappyEngine::~HappyEngine()
{
}

void HappyEngine::quit()
{   
    //dispose/delete all sub engines here
    std::cout << "--Thank you for using HappyEngine--\n";
}
void HappyEngine::initSubEngines()
{
    //init all sub engines here
}

void HappyEngine::start(IGame* pGame)
{
    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n\n\n";

    m_pGame = pGame;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Init Game
    pGame->init();

    //Init other stuff
    initWindow();
    initSubEngines();

    pGame->load();
    
    //Start draw thread
    boost::thread drawThread(boost::bind(&HappyEngine::drawThread, this));

    //Run main update loop
    SDL_Event event;
    Uint32 prevTicks = SDL_GetTicks();
    while (m_Quit == false)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: m_Quit = true; break;
            }
        }
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;

        m_pGame->tick(dTime);

        //SDL_Delay(12);
    }

    drawThread.join();

    SDL_Quit();

    quit();
}
void HappyEngine::drawThread()
{
    Uint32 prevTicks = SDL_GetTicks();
    while (m_Quit == false)
    {
        Uint32 ticks = SDL_GetTicks();
        float dTime = (ticks - prevTicks) / 1000.0f;
        prevTicks = ticks;

        SDL_SetRenderDrawColor(m_pRenderer, 128, 180, 255, 255);
        SDL_RenderClear(m_pRenderer);

        m_pGame->draw(dTime);

        SDL_RenderPresent(m_pRenderer);
    }
}

void HappyEngine::initWindow()
{
    m_pMainWindow = SDL_CreateWindow("Happy Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);
    m_pRenderer = SDL_CreateRenderer(m_pMainWindow, -1, 0);
}

HappyEngine::pointer HappyEngine::getPointer()
{
    return s_pHappyEngine;
}

} //end namespace
