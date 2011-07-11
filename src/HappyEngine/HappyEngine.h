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

#ifndef _HAPPYENGINE_H_
#define _HAPPYENGINE_H_
#pragma once

#include "boost/shared_ptr.hpp"
#include "IGame.h"
#include "SDL.h"

namespace happyengine {

class HappyEngine
{
public:
    typedef boost::shared_ptr<HappyEngine> pointer;

    virtual ~HappyEngine();

    void start(IGame* pGame);

    static pointer getPointer();

private:
    // Singleton design pattern
    HappyEngine();
    static pointer s_pHappyEngine;

    IGame* m_pGame;
    SDL_Window* m_pMainWindow;
    SDL_Renderer* m_pRenderer;

    bool m_Quit;

    // Methods
    void initWindow();
    void initSubEngines();
    void drawThread();
    void quit();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
