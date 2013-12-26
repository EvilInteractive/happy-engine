#include "HappySandBoxPCH.h" 

#include "GameStateLoadPlugin.h"

#include "system/PluginManager.h"
#include "system/EntityManager.h"
#include "system/SelectionManager.h"
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
    he::Path pluginPath("");
    he::pl::IPlugin* const plugin(PluginManager::getInstance()->loadPlugin(pluginPath, "HappyPluginTest"));
    Sandbox* const sandbox(Sandbox::getInstance());
    sandbox->setGamePlugin(plugin);

    SelectionManger::getInstance()->init();

    EntityManager* const entityMan(sandbox->getEntityManager());
    entityMan->init();
    if (plugin != nullptr)
    {
        plugin->init(sandbox->getMainWindow(), he::RectF(0, 0, 1, 1));
    }

    return true;
}

void GameStateLoadPlugin::exit()
{
}

void GameStateLoadPlugin::tick( const float /*dTime*/ )
{

}

} //end namespace
