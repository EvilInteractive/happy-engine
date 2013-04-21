#include "HappySandBoxPCH.h" 

#include "GameStateUnloadPlugin.h"

#include "system/PluginManager.h"
#include "system/EntityManager.h"
#include "system/SelectionManager.h"
#include "Sandbox.h"

#include <IPlugin.h>

namespace hs {

GameStateUnloadPlugin::GameStateUnloadPlugin()
{
}

GameStateUnloadPlugin::~GameStateUnloadPlugin()
{
}

bool GameStateUnloadPlugin::enter()
{
    Sandbox* const sandbox(Sandbox::getInstance());
    he::pl::IPlugin* const plugin(sandbox->getGamePlugin());
    plugin->onUnloadLevel();

    SelectionManger::getInstance()->deselectAll();
    SelectionManger::getInstance()->destroy();

    EntityManager* const entityMan(sandbox->getEntityManager());
    entityMan->destroy();

    plugin->terminate();
    PluginManager::getInstance()->releasePlugin(plugin);
    sandbox->setGamePlugin(nullptr);
    return true;
}

void GameStateUnloadPlugin::exit()
{
}

void GameStateUnloadPlugin::tick( const float /*dTime*/ )
{

}

} //end namespace
