//TheGreatEscape Copyright (C) 2011 Evil Interactive, Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyFxTest.
//
// 
//Author: Bastian Damman
// 
#include "HappyFxTestPCH.h"

#include "Texture2D.h"
#include "MainGame.h"

int main( int /*argc*/, char** /*args[]*/ )
{
    using namespace he;
    using namespace happyfxtest;

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

