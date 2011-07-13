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

#include "GL/glew.h"

#include <vector>

namespace happyengine {

struct VertexPosColor
{
public:
    float x, y, z;
    float r, g, b;

    VertexPosColor(float _x, float _y, float _z,
                   float _r, float _g, float _b) : x(_x), y(_y), z(_z),
                                                   r(_r), g(_g), b(_b)
    {}
};

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
    SDL_GLContext m_GLContext;

    bool m_Quit;
    bool m_Loaded;

    std::vector<VertexPosColor> m_VertexQuad;

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
