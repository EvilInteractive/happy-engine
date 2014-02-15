#include "HappySandBoxPCH.h" 

#include "GameStateMachine.h"

#include "GameState.h"
#include "runtime/GameStateInit.h"
#include "runtime/GameStateExit.h"
#include "runtime/GameStateEdit.h"
#include "runtime/GameStatePlay.h"
#include "runtime/GameStateLoadPlugin.h"
#include "runtime/GameStateUnloadPlugin.h"
#include "runtime/GameStateLoad.h"
#include "runtime/GameStateUnload.h"

#include <Game.h>

#ifdef HE_DEBUG
#define DEBUG_GAMESTATEMACHINE
#endif

namespace hs {

GameStateMachine::GameStateMachine()
    : m_CurrentState(eGameState_Invalid)
{
    he_memset(m_GameStates, 0, sizeof(GameState*) * eGameState_MAX);
}

GameStateMachine::~GameStateMachine()
{
}

void GameStateMachine::init()
{
    m_GameStates[eGameState_Init] = NEW GameStateInit();
    m_GameStates[eGameState_Exit] = NEW GameStateExit();
    m_GameStates[eGameState_Edit] = NEW GameStateEdit();
    m_GameStates[eGameState_Play] = NEW GameStatePlay();
    m_GameStates[eGameState_LoadPlugin] = NEW GameStateLoadPlugin();
    m_GameStates[eGameState_UnloadPlugin] = NEW GameStateUnloadPlugin();
    m_GameStates[eGameState_Load] = NEW GameStateLoad();
    m_GameStates[eGameState_Unload] = NEW GameStateUnload();
    GAME->addToTickList(this);
}

void GameStateMachine::destroy()
{
    for (size_t i(0); i < eGameState_MAX; ++i)
    {
        delete m_GameStates[i];
        m_GameStates[i] = nullptr;
    }
    GAME->removeFromTickList(this);
}


bool GameStateMachine::setState( const EGameState state )
{
#ifdef DEBUG_GAMESTATEMACHINE
    HE_INFO("Setting gamestate to: %s", gameStateToString(state));
#endif
    m_QueuedStates.clear();
    if (m_CurrentState != eGameState_Invalid)
    {
        m_GameStates[m_CurrentState]->exit();
    }
    m_CurrentState = state;
    HE_ASSERT(m_GameStates[m_CurrentState] != nullptr, "Uninitialized gamestate - fatal!");
    return m_GameStates[m_CurrentState]->enter();
}

void GameStateMachine::queueState( const EGameState state )
{
    m_QueuedStates.push_back(state);
}

void GameStateMachine::clearQueue()
{
    m_QueuedStates.clear();
}

void GameStateMachine::tick( float dTime )
{
    if (m_CurrentState != eGameState_Invalid)
    {
        GameState* const currentState(m_GameStates[m_CurrentState]);
        currentState->tick(dTime);
        if (m_QueuedStates.empty() == false && currentState->canExit())
        {
            currentState->exit();
            do 
            {
                if (m_QueuedStates.empty() == false)
                {
                    m_CurrentState = m_QueuedStates.front();
                    m_QueuedStates.pop_front();
#ifdef DEBUG_GAMESTATEMACHINE
                    HE_INFO("Switching gamestate to: %s", gameStateToString(m_CurrentState));
#endif
                    HE_ASSERT(m_GameStates[m_CurrentState] != nullptr, "Uninitialized gamestate - fatal!");
                }
                else
                {
                    m_CurrentState = eGameState_Invalid;
                    LOG(he::LogType_ProgrammerAssert, "Exited state but there is no new state to enter!");
                    break;
                }
            } while (m_GameStates[m_CurrentState]->enter() == false);
        }
    }
}


} //end namespace
