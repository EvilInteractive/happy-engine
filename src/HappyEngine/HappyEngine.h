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

#define SDL_NO_COMPAT
#include "boost/shared_ptr.hpp"
#include "SDL.h"
#include "GL/glew.h"

#include <vector>

#include "IGame.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"

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

    //subengines
    graphics::GraphicsEngine* getGraphicsEngine() const;
    const io::ControlsManager* getControls() const;

private:
    // Singleton design pattern
    HappyEngine();
    static pointer s_pHappyEngine;

    IGame* m_pGame;

    graphics::GraphicsEngine* m_pGraphicsEngine;
    io::ControlsManager* m_pControlsManager;

    bool m_Quit;
    bool m_Loaded;

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
