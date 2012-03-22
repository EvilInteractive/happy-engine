//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#ifndef _MAINGAME_H_
#define _MAINGAME_H_
#pragma once

#include <vector>

#include "HappyQtWidget.h"
#include "Game.h"
#include "FPSGraph.h"
#include "Entity.h"
#include "Grid.h"
#include "HappyFxEditorBinding.h"
#include "Game.h"
#include "SceneInfo.h"
#include "TransformTools.h"

#include "Texture2D.h"

namespace happysandbox {

class MainGame : public he::gfx::HappyQtWidget
{
public:
    MainGame(QWidget* parent = 0);
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);
    virtual void drawGui();

private:

    he::tools::Grid* m_pBaseGrid;

    he::tools::HappyFxEditorBinding* m_pFxEditorBinding;

    SceneInfo* m_pSceneInfo;

    he::gfx::Texture2D::pointer m_pTest;

    TransformTools* m_pTransFormTools;

    /* DEFAULT COPY & ASSIGNMENT */
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
