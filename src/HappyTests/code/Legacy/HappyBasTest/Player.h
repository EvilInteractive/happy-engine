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
//Created: 28/11/2012

#ifndef _HT_Player_H_
#define _HT_Player_H_
#pragma once

#include "Entity.h"

namespace ht {

class PlayerMover;
class PlayerThirdPersonCamera;

class Player : public he::ge::Entity
{
public:
    Player();
    virtual ~Player();

    PlayerThirdPersonCamera* getCamera() const { return m_Camera; }
    PlayerMover* getMover() const { return m_Mover; }

    he::vec3 getCenterPosition() const;

private:
    PlayerThirdPersonCamera* m_Camera;
    PlayerMover* m_Mover;

    //Disable default copy constructor and default assignment operator
    Player(const Player&);
    Player& operator=(const Player&);
};

} //end namespace

#endif
