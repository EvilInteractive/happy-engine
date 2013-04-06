#include "HappySandBoxPCH.h" 

#include "GameState.h"

namespace hs {

GameState::GameState()
{
}

GameState::~GameState()
{
}

const char* gameStateToString( const EGameState state )
{
    const char* result("UNKNOWN STATE");
    switch (state)
    {
        case eGameState_Init: result = "GameState_Init"; break;
        case eGameState_Exit: result = "GameState_Exit"; break;
        case eGameState_Edit: result = "GameState_Edit"; break;
        case eGameState_Play: result = "GameState_Play"; break;
        case eGameState_LoadPlugin: result = "GameState_LoadPlugin"; break;
        case eGameState_UnloadPlugin: result = "GameState_UnloadPlugin"; break;
        case eGameState_Load: result = "GameState_Load"; break;
        case eGameState_Unload: result = "GameState_Unload"; break;
        default: LOG(he::LogType_ProgrammerAssert, "Invalid game state when converting to string: %d", state); break;
    }
    return result;
}

} //end namespace
