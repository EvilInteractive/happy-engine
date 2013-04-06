#ifndef _HS_GameStateInit_H_
#define _HS_GameStateInit_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStateInit : public GameState
{
public:
    GameStateInit();
    virtual ~GameStateInit();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStateInit(const GameStateInit&);
    GameStateInit& operator=(const GameStateInit&);
};

} //end namespace

#endif
