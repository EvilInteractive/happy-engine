#ifndef _HS_GameStateMachine_H_
#define _HS_GameStateMachine_H_
#pragma once

#include "GameState.h"

#include <ITickable.h>
#include <Singleton.h>
#include <deque>

namespace hs {

class GameStateMachine : public he::Singleton<GameStateMachine>, public he::ge::ITickable
{
public:
    GameStateMachine();
    virtual ~GameStateMachine();

    void init();
    void destroy();

    bool setState(const EGameState state);
    void queueState(const EGameState state);
    void clearQueue();

    EGameState getCurrentState() const { return m_CurrentState; }

    virtual void tick(float dTime);

private:
    GameState* m_GameStates[eGameState_MAX];
    EGameState m_CurrentState;
    std::deque<EGameState> m_QueuedStates;

    //Disable default copy constructor and default assignment operator
    GameStateMachine(const GameStateMachine&);
    GameStateMachine& operator=(const GameStateMachine&);
};

} //end namespace

#endif
