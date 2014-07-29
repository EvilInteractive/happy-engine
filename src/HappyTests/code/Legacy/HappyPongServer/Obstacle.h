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
//Created: 30/03/2012

#ifndef _HPS_Obstacle_H_
#define _HPS_Obstacle_H_
#pragma once

#include "Entity.h"
#include "NetworkObject.h"

namespace hps {

class Obstacle : public he::ge::Entity//, public he::net::NetworkObject<Obstacle>
{
public:
    Obstacle();
    virtual ~Obstacle();
    
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    float getRadius() const;
    const he::vec3& getPosition() const;


private:
    he::vec3 m_Position;
    float m_Radius;

    //Disable default copy constructor and default assignment operator
    Obstacle(const Obstacle&);
    Obstacle& operator=(const Obstacle&);
};

} //end namespace

#endif
