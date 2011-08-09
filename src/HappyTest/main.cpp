
#define BOOST_DISABLE_ASSERTS

#include "HappyEngine.h"
#include "MainGame.h"

#include <iostream>

int main( int /*argc*/, char** /*args[]*/ )
{
    using namespace happyengine;
    using namespace happytest;
    
    MainGame* pGame(new MainGame());
    HAPPYENGINE->start(pGame);
    delete pGame;

    #ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif

    std::cout << "press enter to quit\n";
    std::cin.get();

    return 0;
}

