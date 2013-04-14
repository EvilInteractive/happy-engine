#ifndef _HS_GameStateEdit_H_
#define _HS_GameStateEdit_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStateEdit : public GameState
{
public:
    GameStateEdit();
    virtual ~GameStateEdit();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStateEdit(const GameStateEdit&);
    GameStateEdit& operator=(const GameStateEdit&);
};

} //end namespace

#endif
