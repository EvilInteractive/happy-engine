#ifndef _HS_GameStateExit_H_
#define _HS_GameStateExit_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStateExit : public GameState
{
public:
    GameStateExit();
    virtual ~GameStateExit();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStateExit(const GameStateExit&);
    GameStateExit& operator=(const GameStateExit&);
};

} //end namespace

#endif
