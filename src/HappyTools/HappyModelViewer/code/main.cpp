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
//Created: 15/04/2012
#include "HappyModelViewerPCH.h" 

#include "Texture2D.h"
#include "ModelMesh.h"

#include "MainGame.h"
#include "ContentManager.h"

int main( int argc, char** args )
{
    if (argc == 2)
    {

#if _DEBUG && !GCC
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        HAPPYENGINE->init(he::SubEngine_All);

        he::Path workDir(args[0]);
        HAPPYENGINE->setRootDir(workDir.getRelativePath(he::Path("../")));
        CONTENT->setContentDir(HAPPYENGINE->getRootDir().getRelativePath(he::Path("../data")));
        std::string file(args[1]);

        he::game::Game* game(NEW hmv::MainGame(file));
        HAPPYENGINE->start(game);
        delete game;

        HAPPYENGINE->dispose();
    
//#if _DEBUG
//        std::cout << "\npress enter to quit\n";
//        std::cin.get();
//#endif

    }

    return 0;
}
