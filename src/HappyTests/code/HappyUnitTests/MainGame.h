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

#ifndef _HUT_MainGame_H_
#define _HUT_MainGame_H_
#pragma once

#include "Game.h"

namespace hut {

class MainGame : public he::ge::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy();
    virtual void tick(float dTime);

    void nodeGraphUnitTest();
    void listUnitTest();
    void guidUnitTest();
    void mat33UnitTest();
    void jsonUnitTest();
    void threadSafeQueueMP1CTest();
    void midiTest();
    void poolTest();
    void objectHandleTest();
private:

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
