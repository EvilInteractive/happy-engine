#include "HappySandBoxPCH.h" 
#include "GameStateLoad.h"

#include "Sandbox.h"

#include <IPlugin.h>

namespace hs {

GameStateLoad::GameStateLoad()
{
}

GameStateLoad::~GameStateLoad()
{
}

bool GameStateLoad::enter()
{
    Sandbox* const sandbox(Sandbox::getInstance());
    he::pl::IPlugin* const plugin(sandbox->getGamePlugin());
    plugin->onLoadLevel(he::Path(""));
    return true;
}

void GameStateLoad::exit()
{
}

void GameStateLoad::tick( const float /*dTime*/ )
{

}

} //end namespace
