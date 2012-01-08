//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "HeAssert.h"
#define assert ASSERT

#include "boost/shared_ptr.hpp"

#include "HappyEngine.h"
#include "MainGame.h"
#include "Texture2D.h"
#include "ModelMesh.h"

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

    std::cout << "\nallocated textures: " << gfx::Texture2D::getTextureCount();
    std::cout << "\nallocated modelMeshes: " << gfx::ModelMesh::getAllocatedModelMeshCount() << "\n";

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}

