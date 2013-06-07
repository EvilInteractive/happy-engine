#include "HappySandBoxPCH.h" 
#include "GameStateUnload.h"

#include "Sandbox.h"

#include <IPlugin.h>

namespace hs {

GameStateUnload::GameStateUnload ()
{
}

GameStateUnload::~GameStateUnload ()
{
}

bool GameStateUnload::enter()
{
    Sandbox* const sandbox(Sandbox::getInstance());
    he::pl::IPlugin* const plugin(sandbox->getGamePlugin());
    plugin->onUnloadLevel();
    return true;
}

void GameStateUnload::exit()
{
}

void GameStateUnload::tick( const float /*dTime*/ )
{

}

} //end namespace
