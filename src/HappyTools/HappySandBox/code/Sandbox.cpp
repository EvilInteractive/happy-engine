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
//
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#include "HappySandBoxPCH.h" 
#include "Sandbox.h"

#include "system/EntityManager.h"
#include "system/SandboxRenderPipeline.h"
#include "system/GameStateMachine.h"
#include "system/EditorPickingManager.h"
#include "system/SelectionManager.h"
#include "StaticDataManager.h"

#include "forms/MainWindow.h"
#include "forms/GameWidget.h"

#include <qapplication.h>
#include <QColorDialog>

#include <ControlsManager.h>
#include <ContentManager.h>
#include <FPSGraph.h>
#include <View.h>
#include <PluginLoader.h>
#include <EntityManager.h>
#include <ControlsManager.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <GraphicsEngine.h>
#include <Ray.h>
#include <PickResult.h>
#include <CameraPerspective.h>
#include <PickingComponent.h>
#include <IPlugin.h>
#include <GlobalSettings.h>


namespace hs {

Sandbox::Sandbox():   m_RenderPipeline(nullptr),
                      m_View(nullptr),
                      m_Window(nullptr),
                      m_GamePlugin(nullptr),
                      m_EntityManager(nullptr),
                      m_IsExiting(false),
                      m_ColorPicker(nullptr)
{
}

Sandbox::~Sandbox()
{
}

int Sandbox::run(int argc, char* args[])
{
    QApplication::setStyle("plastique");
    QApplication app(argc, args);
    app.setQuitOnLastWindowClosed(false);

    QGLFormat glwformat;
    glwformat.setVersion( 3, 2 );
    glwformat.setProfile( QGLFormat::CoreProfile );
    QGLFormat::setDefaultFormat(glwformat);

    HE_ASSERT(QGLFormat::defaultFormat().majorVersion() == 3, "Default Major is not 3! but %d", QGLFormat::defaultFormat().majorVersion());

    hs::StaticDataManager::init();
    he::HappyEngine::init(argc, args, he::SubEngine_All & ~he::SubEngine_Windowing);

    m_Window = NEW MainWindow();
    connect(m_Window, SIGNAL(close()), this, SLOT(quit()));
    m_Window->show();
    m_Window->getGameWidget()->create(true);

    HAPPYENGINE->start(this, false, m_Window->getGameWidget());
    init();

    m_QtLoopTimer.setSingleShot(true); //as fast as possible
    connect(&m_QtLoopTimer, SIGNAL(timeout()), this, SLOT(loop()));
    m_QtLoopTimer.start(12);

    int ret = app.exec();

    destroy();

    m_Window->getGameWidget()->destroy(); // Unregister context and stuff

    he::HappyEngine::dispose();

    delete m_Window;
    m_Window = nullptr;

    hs::StaticDataManager::destroy();

    return ret;
}

void Sandbox::loop()
{
    HAPPYENGINE->loop();
}

void Sandbox::destroy()
{
    delete m_ColorPicker;
    m_ColorPicker = nullptr;

    delete m_EntityManager;
    m_EntityManager = nullptr;
    
    delete m_RenderPipeline;
    m_RenderPipeline = nullptr;

    GRAPHICS->removeView(m_View);
    m_View = nullptr;

    GRAPHICS->unregisterWindow(m_Window->getGameWidget());

    GameStateMachine* const stateMachine(GameStateMachine::getInstance());
    stateMachine->destroy();
}

void Sandbox::init()
{
    he::GlobalSettings* const globalSettings(he::GlobalSettings::getInstance());
    globalSettings->load(he::Path("sandboxSettings.cfg"));
    globalSettings->save(he::Path("sandboxSettings.cfg"));

    GRAPHICS->registerWindow(m_Window->getGameWidget());
    m_View = GRAPHICS->createView();
    
    using namespace he;

    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->setWindow(m_Window->getGameWidget());

    m_RenderPipeline = NEW SandboxRenderPipeline();
    m_RenderPipeline->init(m_View);
    m_View->init(cameraSettings);
    m_EntityManager = NEW EntityManager();

    GameStateMachine* const stateMachine(GameStateMachine::getInstance());
    stateMachine->init();
    stateMachine->setState(eGameState_Init);

    m_ColorPicker = NEW QColorDialog(m_Window);
}

void Sandbox::tick(float dTime)
{
    he::ge::Game::tick(dTime);

    if (!HAPPYENGINE->isQuiting())
        m_QtLoopTimer.start(12);
}

void Sandbox::quit()
{
    if (m_IsExiting == false)
    {
        m_IsExiting = true;
        GameStateMachine* const stateMachine(GameStateMachine::getInstance());
        stateMachine->clearQueue();
        switch (stateMachine->getCurrentState())
        {
            case eGameState_Init: 
            case eGameState_UnloadPlugin:
            { 
                stateMachine->queueState(eGameState_Exit);
            } break;
            case eGameState_Exit:
            { 
            } break;
            case eGameState_Edit:
            case eGameState_Play:
            case eGameState_Load:
            { 
                stateMachine->queueState(eGameState_Unload);
                stateMachine->queueState(eGameState_UnloadPlugin);
                stateMachine->queueState(eGameState_Exit);
            } break;
            case eGameState_LoadPlugin:
            case eGameState_Unload:
            { 
                stateMachine->queueState(eGameState_UnloadPlugin);
                stateMachine->queueState(eGameState_Exit);
            } break;
            default:
            {
                LOG(he::LogType_ProgrammerAssert, "Unknown state to exit from: %s", gameStateToString(stateMachine->getCurrentState()));
                break;
            }
        }
    }
}

he::gfx::Window*Sandbox::getMainWindow() const
{
    return m_Window->getGameWidget();
}

QGLWidget* Sandbox::getSharedWidget() const
{
    QGLWidget* sharedWidget(nullptr);
    he::gfx::GraphicsEngine* gfxEngine(GRAPHICS);

    if (gfxEngine)
    {
        sharedWidget = he::checked_cast<GameWidget*>(gfxEngine->getSharedContext());
    }

    return nullptr;
}

} //end namespace
