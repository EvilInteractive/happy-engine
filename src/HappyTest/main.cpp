
#define BOOST_DISABLE_ASSERTS

#include "HappyEngine.h"
#include "MainGame.h"

#include <iostream>

int main( int /*argc*/, char** /*args[]*/ )
{
    using namespace happyengine;
    using namespace happytest;

    HappyEngine::pointer pEngine(HappyEngine::getPointer());

    MainGame* pGame(new MainGame());
    pEngine->start(pGame);
    delete pGame;

    std::cout << "press enter to quit\n";
    std::cin.get();

    return 0;
}

