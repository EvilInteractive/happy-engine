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

#ifndef _HE_HAPPYENGINE_H_
#define _HE_HAPPYENGINE_H_
#pragma once

#include "Random.h"
#include <chrono>

namespace he {
namespace gfx {
    class GraphicsEngine;
    class ShapeRenderer;
    class Renderer2D;
    class GLContext;
    class Window;
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

#define HAPPYENGINE he::gEngine
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

class HappyEngine;
extern HAPPY_ENTRY HappyEngine* gEngine;

enum SubEngine
{
    SubEngine_None = 0,
    SubEngine_Graphics = 1 << 0,
    SubEngine_Physics = 1 << 1,
    SubEngine_Networking = 1 << 2,
    SubEngine_Audio = 1 << 3,
    SubEngine_Controls = 1 << 4,
    SubEngine_Windowing = 1 << 5,
    SubEngine_All = 0xff
};

class HAPPY_ENTRY HappyEngine
{
public:
    ~HappyEngine();

    static void init(const int argc, const char* const* const argv, const int subengines);
    static void dispose();

    // Start the engine and run the game
    // if managed, the engine will maintain the game loop and
    // call game init and destroy before and after the loop
    // if not managed, the method will immediately return, and you
    // should maintain the game loop and call loop()
    void start(ge::Game* game, const bool managed, he::gfx::Window* sharedContext = nullptr);
    void quit();
    bool isQuiting() { return m_Quit; }

    // Do not call unless you started the game unmanaged
    void loop();

    // Do not call
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

    std::chrono::high_resolution_clock::time_point m_PrevTime;
    
    // Methods
    void initWindow();
    void updateLoop(float dTime);
    void drawLoop();
    void cleanup();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
