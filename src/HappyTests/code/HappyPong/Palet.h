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
//Created: 30/03/2012

#ifndef _HT_Palet_H_
#define _HT_Palet_H_
#pragma once

#include "Entity.h"
#include "ITickable.h"

namespace ht {
class LightFlashComponent;
class MainGame;
class Palet : public he::ge::Entity, public he::ge::ITickable
{
public:
    Palet(const MainGame* ge, he::byte player, bool ai);
    virtual ~Palet();

    //////////////////////////////////////////////////////////////////////////
    // he::ge::ITickable
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    const he::vec2& getDimension() const;
    const he::vec3& getPosition() const;

    void hitByBall();
    void addPoint();

private:
    he::vec3 m_Position;
    float m_Speed;

    const he::vec2 m_PaletDim;

    LightFlashComponent* m_LightFlashComponent;
    LightFlashComponent* m_LightFlashAddPointComponent;

    he::byte m_Player;
    bool m_Ai;

    const MainGame* m_Game;

    //Disable default copy constructor and default assignment operator
    Palet(const Palet&);
    Palet& operator=(const Palet&);
};

} //end namespace

#endif
