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
#include "HappyTestsPCH.h" 

#include "Texture2D.h"
#include "ModelMesh.h"

#include "MainGame.h"

// Happy Bas test

int main( int /*argc*/, char** /*args[]*/ )
{

#if _DEBUG && !GCC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    HAPPYENGINE->init(he::SubEngine_All);

    he::game::Game* game(NEW ht::MainGame());
    HAPPYENGINE->start(game);
    delete game;

    HAPPYENGINE->dispose();

    std::cout << "\nallocated textures:    " << he::gfx::Texture2D::getTextureCount();
    std::cout << "\nallocated modelMeshes: " << he::gfx::ModelMesh::getAllocatedModelMeshCount() << "\n";

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}
