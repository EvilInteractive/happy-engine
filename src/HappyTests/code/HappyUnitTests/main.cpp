//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 03/11/2012
#include "HappyUnitTestsPCH.h"

#include "MainGame.h"
#include "UnitTestFixedStrings.h"

int main( int argc, char* args[] )
{
#if defined(HE_WINDOWS) && defined(HE_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    he::HappyEngine::init(argc, args, he::SubEngine_All);
    hut::HTFS::sdmInit();

    hut::MainGame* game(NEW hut::MainGame());
    game->objectHandleTest();
    HAPPYENGINE->start(game, true);
    delete game;

    hut::HTFS::sdmDestroy();
    he::HappyEngine::dispose();

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}
