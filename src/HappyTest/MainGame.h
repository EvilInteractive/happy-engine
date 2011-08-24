//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#define BOOST_DISABLE_ASSERTS


#include "IGame.h"
#include "Deferred3DRenderer.h"
#include "TestObject.h"
#include "Color.h"
#include "DeferredPreEffect.h"
#include "TestBullet.h"

#include <ppl.h>
#include <concurrent_vector.h>
#include <vector>

namespace happytest {

class MainGame : public happyengine::IGame
{
public:
	MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);
    virtual void draw(float dTime);

private:
    happyengine::graphics::Deferred3DRenderer* m_pDeferred3DRenderer;
    TestObject* m_pTestObject;

    happyengine::Color m_BackgroundColors[5];
    happyengine::byte m_BackgroundIndex;

    DeferredPreEffect* m_pDeferredPreEffect;

    float m_DrawTimer;
    float m_UpdateTimer;
    
    Concurrency::concurrent_vector<TestBullet*> m_Bullets;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
