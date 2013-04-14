#include "HappySandBoxPCH.h" 

#include "GameStateInit.h"
#include "Sandbox.h"
#include "system/SandboxRenderPipeline.h"
#include "system/UIManager.h"
#include "system/GameStateMachine.h"

namespace hs {

GameStateInit::GameStateInit()
{
}

GameStateInit::~GameStateInit()
{
}

bool GameStateInit::enter()
{
    Sandbox* const sandbox(Sandbox::getInstance());
    DebugRenderer* const debugRenderer(sandbox->getRenderPipeline()->getDebugRenderer());
    CONSOLE->attachToRenderer(debugRenderer);
    PROFILER->attachToRenderer(debugRenderer);

    UIManager* const uiManager(UIManager::getInstance());
    uiManager->load();
    uiManager->showDebugUI();
    uiManager->showEditorUI();
    
    GameStateMachine::getInstance()->queueState(eGameState_LoadPlugin);

    return true;
}

void GameStateInit::exit()
{
}

void GameStateInit::tick( const float /*dTime*/ )
{

}

} //end namespace
