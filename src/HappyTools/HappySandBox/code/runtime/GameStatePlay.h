#ifndef _HS_GameStatePlay_H_
#define _HS_GameStatePlay_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStatePlay : public GameState
{
public:
    GameStatePlay();
    virtual ~GameStatePlay();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStatePlay(const GameStatePlay&);
    GameStatePlay& operator=(const GameStatePlay&);
};

} //end namespace

#endif
