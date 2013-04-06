#ifndef _HS_GameStateUnloadPlugin_H_
#define _HS_GameStateUnloadPlugin_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStateUnloadPlugin : public GameState
{
public:
    GameStateUnloadPlugin();
    virtual ~GameStateUnloadPlugin();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStateUnloadPlugin(const GameStateUnloadPlugin&);
    GameStateUnloadPlugin& operator=(const GameStateUnloadPlugin&);
};

} //end namespace

#endif
