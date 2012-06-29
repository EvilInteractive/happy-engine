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
#include "HappyPongClientPCH.h" 

#include "Texture2D.h"
#include "ModelMesh.h"
#include "ContentManager.h" 
#include "HPCStaticDataManager.h"

#include "MainGame.h"

int main( int /*argc*/, char** /*args[]*/ )
{

#if _DEBUG && !GCC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    HAPPYENGINE->init(he::SubEngine_Graphics   | 
                      he::SubEngine_Content    | 
                      he::SubEngine_2DRenderer | 
                      he::SubEngine_Controls   | 
                      he::SubEngine_Networking |
                      he::SubEngine_Audio      );
    CONTENT->setContentDir(he::Path("../../data"));

    hpc::HPCStaticDataManager::init();

    hpc::MainGame::sdmInit();
    HAPPYENGINE->start(hpc::MainGame::getInstance());
    hpc::MainGame::sdmDestroy();

    hpc::HPCStaticDataManager::destroy();

    HAPPYENGINE->dispose();

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}
