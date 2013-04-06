#include "HappySandBoxPCH.h" 

#include "GameStateExit.h"

#include "Sandbox.h"
#include "system/SandboxRenderPipeline.h"
#include "system/UIManager.h"

#include <tools/FPSGraph.h>

namespace hs {

GameStateExit::GameStateExit ()
{
}

GameStateExit::~GameStateExit ()
{
}

bool GameStateExit::enter()
{
    UIManager* const uiManager(UIManager::getInstance());
    uiManager->unload();

    CONSOLE->detachFromRenderer();
    PROFILER->detachFromRenderer();
    HAPPYENGINE->quit();
    return true;
}

void GameStateExit::exit()
{
}

void GameStateExit::tick( const float /*dTime*/ )
{

}

} //end namespace
