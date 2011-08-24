#include "Assert.h"
#define assert ASSERT

#include "boost/shared_ptr.hpp"

#include "HappyEngine.h"
#include "MainGame.h"
#include "Texture2D.h"

#include "HappyNew.h"

#include <iostream>

int main( int /*argc*/, char** /*args[]*/ )
{
    using namespace happyengine;
    using namespace happytest;
    
    MainGame* pGame(NEW MainGame());
    HAPPYENGINE->start(pGame);
    delete pGame;
    HAPPYENGINE->dispose();

    std::cout << "\nallocated textures: " << graphics::Texture2D::getTextureCount() << "\n";

    #ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}

