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

#ifndef _HE_HAPPYENGINE_H_
#define _HE_HAPPYENGINE_H_
#pragma once

#include "Random.h"

namespace he {
namespace gfx {
    class GraphicsEngine;
    class ShapeRenderer;
    class Renderer2D;
}
namespace gui {
    struct Gui;
}
namespace io {
    class ControlsManager;
}
namespace px {
    class PhysicsEngine;
}
namespace ct {
    class ContentManager;
}
namespace net {
    class NetworkManager;
}
namespace tools {
    class Console;
}
namespace sfx {
    class SoundEngine;
}
namespace ge {
    class Game;
    class CameraManager;
}
namespace pl {
    class PluginLoader;
}
}

#define HAPPYENGINE he::HappyEngine::getInstance()
#define GRAPHICS HAPPYENGINE->getGraphicsEngine()
#define CONTROLS HAPPYENGINE->getControls()
#define PHYSICS HAPPYENGINE->getPhysics()
#define CONTENT HAPPYENGINE->getContentManager()
#define NETWORK HAPPYENGINE->getNetworkManager()
#define CONSOLE HAPPYENGINE->getConsole()
#define AUDIO HAPPYENGINE->getSoundEngine()
#define GAME HAPPYENGINE->getGame()
#define GUI HAPPYENGINE->getGui()

namespace he {
enum SubEngine
{
    SubEngine_None = 0,
    SubEngine_Graphics = 1 << 0,
    SubEngine_Physics = 1 << 1,
    SubEngine_Networking = 1 << 2,
    SubEngine_Audio = 1 << 3,
    SubEngine_All = 1<<0 | 1<<1 | 1<<2 | 1<<3
};

class HAPPY_ENTRY HappyEngine : public Singleton<HappyEngine>
{
friend class Singleton<HappyEngine>;
public:
    virtual ~HappyEngine();

    static void init(const int subengines, const he::Path& dataPath);
    static void dispose();

    void start(ge::Game* pGame);
        
    void quit();
    
    void audioLoop();

    //root dir
    void setRootDir(const Path& rootDir) { m_RootDir = rootDir; }
    const Path& getRootDir() const { return m_RootDir; } //includes trailing slash

    //subengines
    gfx::GraphicsEngine* getGraphicsEngine() const { return m_GraphicsEngine; }
    io::ControlsManager* getControls() const { return m_ControlsManager; }
    px::PhysicsEngine* getPhysics() const { return m_PhysicsEngine; }
    ct::ContentManager* getContentManager() const { return m_ContentManager; }
    net::NetworkManager* getNetworkManager() const { return m_NetworkManager; }
    tools::Console* getConsole() const { return m_Console; }
    sfx::SoundEngine* getSoundEngine() const { return m_SoundEngine; }
    ge::Game* getGame() const { return m_Game; }
    gui::Gui* getGui() const { return m_Gui; }
    pl::PluginLoader* getPluginLoader() const { return m_PluginLoader; }
    
private:
    // Singleton design pattern
    HappyEngine();

    void initSubEngines(int subengines);
    
    ge::Game* m_Game;

    gfx::GraphicsEngine* m_GraphicsEngine;

    io::ControlsManager* m_ControlsManager;
    px::PhysicsEngine* m_PhysicsEngine;
    ct::ContentManager* m_ContentManager;
    net::NetworkManager* m_NetworkManager;
    tools::Console* m_Console;
    sfx::SoundEngine* m_SoundEngine;
    gui::Gui* m_Gui;
    pl::PluginLoader* m_PluginLoader;

    Path m_RootDir;

    bool m_Quit;
    bool m_ShowProfiler;
    bool m_GameLoading;

    int m_SubEngines;

    he::Thread m_AudioThread;

    boost::chrono::high_resolution_clock::time_point m_PrevTime;
    
    // Methods
    void initWindow();
    void loop();
    void updateLoop(float dTime);
    void drawLoop();
    void cleanup();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
