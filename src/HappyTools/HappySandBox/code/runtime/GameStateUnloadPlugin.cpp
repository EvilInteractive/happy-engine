#include "HappySandBoxPCH.h" 

#include "GameStateUnloadPlugin.h"

namespace hs {

GameStateUnloadPlugin::GameStateUnloadPlugin()
{
}

GameStateUnloadPlugin::~GameStateUnloadPlugin()
{
}

bool GameStateUnloadPlugin::enter()
{
    return true;
}

void GameStateUnloadPlugin::exit()
{
}

void GameStateUnloadPlugin::tick( const float /*dTime*/ )
{

}

} //end namespace
