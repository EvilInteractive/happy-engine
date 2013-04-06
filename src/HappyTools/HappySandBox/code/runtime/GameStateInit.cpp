#include "HappySandBoxPCH.h" 

#include "GameStateInit.h"
#include "Sandbox.h"
#include "system/SandboxRenderPipeline.h"

namespace hs {

GameStateInit::GameStateInit ()
{
}

GameStateInit::~GameStateInit ()
{
}

bool GameStateInit::enter()
{
    Sandbox* const sandbox(Sandbox::getInstance());
    he::gfx::Renderer2D* const debugRenderer(sandbox->getRenderPipeline()->getDebugRenderer());
    CONSOLE->attachToRenderer(debugRenderer);
    PROFILER->attachToRenderer(debugRenderer);
    return true;
}

void GameStateInit::exit()
{
}

void GameStateInit::tick( const float /*dTime*/ )
{

}

} //end namespace
