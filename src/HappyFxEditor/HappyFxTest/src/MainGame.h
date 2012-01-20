//TheGreatEscape Copyright (C) 2011 Evil Interactive, Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyFxTest.
//
// 
//Author: Bastian Damman
// 

#ifndef _HFX_MAIN_GAME_H_
#define _HFX_MAIN_GAME_H_
#pragma once

#include "Game.h"
#include "FPSGraph.h"
#include "SkyBox.h"
#include "Entity.h"
#include "HappyFxEditorBinding.h"

namespace happyfxtest {
    
class MainGame : public he::game::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void drawGui();

    virtual void tick(float dTime);

private:

    he::tools::FPSGraph* m_pFpsGraph;
    he::gfx::SkyBox* m_pSkyBox;
    he::game::Entity* m_pGround;
    he::tools::HappyFxEditorBinding* m_pFxEditorBinding;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
