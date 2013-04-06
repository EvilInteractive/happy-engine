#include "HappySandBoxPCH.h" 

#include "GameStateLoadPlugin.h"

#include "system/PluginManager.h"
#include "system/EntityManager.h"
#include "Sandbox.h"

#include <IPlugin.h>

namespace hs {

GameStateLoadPlugin::GameStateLoadPlugin()
{
}

GameStateLoadPlugin::~GameStateLoadPlugin()
{
}

bool GameStateLoadPlugin::enter()
{
    he::Path pluginPath("HappyPluginTest.dll");
    he::pl::IPlugin* const plugin(PluginManager::getInstance()->loadPlugin(pluginPath));
    Sandbox* const sandbox(Sandbox::getInstance());
    sandbox->setGamePlugin(plugin);

    EntityManager* const entityMan(sandbox->getEntityManager());
    entityMan->init();
    plugin->init(sandbox->getMainWindow(), he::RectF(0, 0, 1, 1));
    plugin->onLoadLevel(he::Path(""));

    return true;
}

void GameStateLoadPlugin::exit()
{
}

void GameStateLoadPlugin::tick( const float /*dTime*/ )
{

}

} //end namespace
