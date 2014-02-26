//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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

#include "ControlsManager.h"

#include "FPSGraph.h"

#include "Window.h"
#include "View.h"

#include "PluginLoader.h"
#include "system/EntityManager.h"
#include "system/SandboxRenderPipeline.h"
#include "system/GameStateMachine.h"
#include "system/EditorPickingManager.h"
#include "system/SelectionManager.h"

#include <EntityManager.h>
#include <ControlsManager.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <materialGenerator/MaterialGeneratorGraph.h>
#include <GraphicsEngine.h>
#include <Ray.h>
#include <PickResult.h>
#include <CameraPerspective.h>
#include <PickingComponent.h>
#include <IPlugin.h>
#include <GlobalSettings.h>


namespace hs {

Sandbox::Sandbox():   m_RenderPipeline(nullptr),
                      m_Window(nullptr),
                      m_View(nullptr),
                      m_GamePlugin(nullptr),
                      m_EntityManager(nullptr),
                      m_MaterialGenerator(nullptr),
                      m_IsExiting(false)
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::destroy()
{
    delete m_EntityManager;
    m_EntityManager = nullptr;

    delete m_MaterialGenerator;
    m_MaterialGenerator = nullptr;
    
    delete m_RenderPipeline;
    m_RenderPipeline = nullptr;

    GRAPHICS->removeView(m_View);
    m_View = nullptr;

    GRAPHICS->removeWindow(m_Window);
    m_Window = nullptr;

    GameStateMachine* const stateMachine(GameStateMachine::getInstance());
    stateMachine->destroy();
}

void Sandbox::init()
{
    he::GlobalSettings* const globalSettings(he::GlobalSettings::getInstance());
    globalSettings->load(he::Path("sandboxSettings.cfg"));
    globalSettings->save(he::Path("sandboxSettings.cfg"));

    m_View = GRAPHICS->createView();
    m_Window = GRAPHICS->createWindow();

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("Happy Sandbox");
    he::eventCallback0<void> quitHandler(std::bind(&Sandbox::quit, this));
    m_Window->Closed += quitHandler;
    m_Window->create();
    
    using namespace he;

    he::gfx::CameraSettings cameraSettings;
    cameraSettings.setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->setWindow(m_Window);

    m_RenderPipeline = NEW SandboxRenderPipeline();
    m_RenderPipeline->init(m_View);
    m_View->init(cameraSettings);
            
    m_MaterialGenerator = NEW he::tools::MaterialGeneratorGraph();
    m_MaterialGenerator->init();

    m_EntityManager = NEW EntityManager();

    GameStateMachine* const stateMachine(GameStateMachine::getInstance());
    stateMachine->init();
    stateMachine->setState(eGameState_Init);
}

void Sandbox::tick(float dTime)
{
    he::io::ControlsManager* const controls(CONTROLS);
    he::io::IKeyboard* const keyboard(controls->getKeyboard());
    if (keyboard->isKeyPressed(he::io::Key_F9))
    {
        if (m_MaterialGenerator->isOpen())
            m_MaterialGenerator->close();
        else
            m_MaterialGenerator->open();
    }
    he::ge::Game::tick(dTime);
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

} //end namespace
