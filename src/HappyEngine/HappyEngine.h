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

#if _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#if _DEBUG
    #undef new
#endif
#include "boost/thread.hpp"
#if _DEBUG
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define SDL_NO_COMPAT
#include "boost/shared_ptr.hpp"
#include "SDL.h"
#include "GL/glew.h"

#include <vector>

#include "IGame.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "PhysicsEngine.h"

#define HAPPYENGINE happyengine::HappyEngine::getPointer()
#define GRAPHICS HAPPYENGINE->getGraphicsEngine()
#define CONTROLS HAPPYENGINE->getControls()
#define PHYSICS HAPPYENGINE->getPhysics()

namespace happyengine {

class HappyEngine
{
public:
    typedef boost::shared_ptr<HappyEngine> pointer;

    virtual ~HappyEngine();

    void start(IGame* pGame);
    void dispose();

    static pointer getPointer();

    void quit();

    //subengines
    graphics::GraphicsEngine* getGraphicsEngine() const;
    const io::ControlsManager* getControls() const;
    physics::PhysicsEngine* getPhysics() const;

private:
    // Singleton design pattern
    HappyEngine();
    static pointer s_pHappyEngine;

    IGame* m_pGame;

    graphics::GraphicsEngine* m_pGraphicsEngine;
    io::ControlsManager* m_pControlsManager;
    physics::PhysicsEngine* m_pPhysicsEngine;

    bool m_Quit;
    bool m_Loaded;

    // Methods
    void initWindow();
    void initSubEngines();
    void updateLoop();
    void drawLoop();
    void cleanup();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
