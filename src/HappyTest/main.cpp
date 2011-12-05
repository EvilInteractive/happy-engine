#include "HeAssert.h"
#define assert ASSERT

#include "boost/shared_ptr.hpp"

#include "HappyEngine.h"
#include "MainGame.h"
#include "Texture2D.h"

#include "HappyNew.h"

#include <iostream>

int main( int /*argc*/, char** /*args[]*/ )
{
    using namespace he;
    using namespace happytest;

    #if _DEBUG && !GCC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    HAPPYENGINE->init(SubEngine_All);
    MainGame* pGame(NEW MainGame());
    HAPPYENGINE->start(pGame);
    delete pGame;
    HAPPYENGINE->dispose();

    std::cout << "\nallocated textures: " << gfx::Texture2D::getTextureCount() << "\n";

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}

