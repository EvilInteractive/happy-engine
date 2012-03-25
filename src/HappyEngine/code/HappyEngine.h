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

namespace he {
namespace gfx {
    class GraphicsEngine;
    class Happy2DRenderer;
    class HappyQtWidget;
    class ExtraForward3DRenderer;
    class FxManager;
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
    class LoadingScreen;
}
namespace sfx {
    class SoundEngine;
}
namespace game {
    class Game;
    class CameraManager;
}
}

#define HAPPYENGINE he::HappyEngine::getPointer()
#define GRAPHICS HAPPYENGINE->getGraphicsEngine()
#define CONTROLS HAPPYENGINE->getControls()
#define PHYSICS HAPPYENGINE->getPhysics()
#define CONTENT HAPPYENGINE->getContentManager()
#define NETWORK HAPPYENGINE->getNetworkManager()
#define GUI HAPPYENGINE->get2DRenderer()
#define CONSOLE HAPPYENGINE->getConsole()
#define AUDIO HAPPYENGINE->getSoundEngine()
#define GAME HAPPYENGINE->getGame()
#define CAMERAMANAGER HAPPYENGINE->getCameraManager()
#define HE3DX HAPPYENGINE->get3DRenderer()
#define FX HAPPYENGINE->getFxManager()

namespace he {
enum SubEngine
{
    SubEngine_None = 0,
    SubEngine_Graphics = 1 << 0,
    SubEngine_Physics = 1 << 1,
    SubEngine_Networking = 1 << 2,
    SubEngine_Controls = 1 << 3,
    SubEngine_Content = 1 << 4,
    SubEngine_2DRenderer = 1 << 5,
    SubEngine_Audio = 1 << 6,
    SubEngine_Qt = 1 << 7,
    SubEngine_All = 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6
};
#ifdef HE_ENABLE_QT
class HappyEngine  : public /*qt::*/QObject
{
    Q_OBJECT
#else
class HappyEngine
{
#endif
public:
    virtual ~HappyEngine();

    static void init(int subengines);
    void start(game::Game* pGame);
#ifdef HE_ENABLE_QT
    void start(gfx::HappyQtWidget* pWidget);
#endif
    static void dispose();

    static HappyEngine* getPointer();

    void quit();

    const std::vector<SDL_Event>& getSDLEvents() const;

    void audioLoop();

    //subengines
    gfx::GraphicsEngine* getGraphicsEngine() const;
    const io::ControlsManager* getControls() const;
    px::PhysicsEngine* getPhysics() const;
    ct::ContentManager* getContentManager() const;
    net::NetworkManager* getNetworkManager() const;
    gfx::Happy2DRenderer* get2DRenderer() const;
    tools::Console* getConsole() const;
    sfx::SoundEngine* getSoundEngine() const;
    game::Game* getGame() const;
    game::CameraManager* getCameraManager() const;
    gfx::ExtraForward3DRenderer* get3DRenderer() const;
    gfx::FxManager* getFxManager() const;

private:
    // Singleton design pattern
    HappyEngine();
    static HappyEngine* s_pHappyEngine;
    void initSubEngines(int subengines);

    void drawLoadingScreen();

    tools::LoadingScreen* m_pLoadingScreen;

    game::Game* m_pGame;
    game::CameraManager* m_pCameraManager;

    gfx::GraphicsEngine* m_pGraphicsEngine;
    gfx::Happy2DRenderer* m_p2DRenderer;
    gfx::ExtraForward3DRenderer* m_p3DRenderer;
    gfx::FxManager* m_pFxManager;

    io::ControlsManager* m_pControlsManager;
    px::PhysicsEngine* m_pPhysicsEngine;
    ct::ContentManager* m_pContentManager;
    net::NetworkManager* m_pNetworkManager;
    tools::Console* m_pConsole;
    sfx::SoundEngine* m_pSoundEngine;

    bool m_Quit;
    bool m_bShowProfiler;
    bool m_bGameLoading;

    int m_SubEngines;

    std::vector<SDL_Event> m_SDLEvents;
    boost::thread m_AudioThread;

    boost::chrono::high_resolution_clock::time_point m_PrevTime;

    // Methods
    void initWindow();
    void loop();
private:
    void updateLoop(float dTime);
    void drawLoop();
    void cleanup();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
