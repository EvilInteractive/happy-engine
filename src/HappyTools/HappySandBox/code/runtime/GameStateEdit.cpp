#include "HappySandBoxPCH.h" 

#include "GameStateEdit.h"
#include "Sandbox.h"
#include "system/UIManager.h"

#include <IPlugin.h>

namespace hs {

GameStateEdit::GameStateEdit()
{
}

GameStateEdit::~GameStateEdit()
{
}

bool GameStateEdit::enter()
{
    he::pl::IPlugin* const plugin(Sandbox::getInstance()->getGamePlugin());
    plugin->onPauseGame();

    UIManager::getInstance()->showEditorUI();

    return true;
}

void GameStateEdit::exit()
{
}

void GameStateEdit::tick( const float /*dTime*/ )
{

}

} //end namespace
