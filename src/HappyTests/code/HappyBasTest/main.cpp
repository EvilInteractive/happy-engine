//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 20/03/2012
#include "HappyBasTestPCH.h" 

#include "Texture2D.h"
#include "ModelMesh.h"
#include "ContentManager.h"

#include "MainGame.h"

// Happy Bas test

int main( int /*argc*/, char** /*args[]*/ )
{

#if _DEBUG && !GCC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(2988);
#endif

    HAPPYENGINE->init(he::SubEngine_All);
    CONTENT->setContentDir(he::Path("../../data"));

    he::ge::Game* ge(NEW ht::MainGame());
    HAPPYENGINE->start(ge);
    delete ge;

    HAPPYENGINE->dispose();

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}
