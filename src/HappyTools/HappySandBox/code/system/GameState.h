#ifndef _HS_GameState_H_
#define _HS_GameState_H_
#pragma once

namespace hs {

enum EGameState
{
    eGameState_Init,
    eGameState_Exit,
    eGameState_Edit,
    eGameState_Play,
    eGameState_LoadPlugin,
    eGameState_UnloadPlugin,
    eGameState_Load,
    eGameState_Unload,
    

    eGameState_MAX,
    eGameState_Invalid = eGameState_MAX
};

const char* gameStateToString(const EGameState state);

class GameState
{
public:
    GameState();
    virtual ~GameState();

    // return false to immediately enter to the next queued state
    // if no states are queued an assert will be thrown
    virtual bool enter() { return true; }
    virtual void exit() {}
    virtual bool canExit() const { return true; }

    virtual void tick(const float /*dTime*/) {}

private:

    //Disable default copy constructor and default assignment operator
    GameState(const GameState&);
    GameState& operator=(const GameState&);
};

} //end namespace

#endif
