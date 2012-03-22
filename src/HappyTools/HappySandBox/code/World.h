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
//
//Author:  Sebastiaan Sprengers
//Created: 12/01/2012

#ifndef _WORLD_H_
#define _WORLD_H_
#pragma once

#include "Entity.h"
#include <vector>

namespace happysandbox {

class World
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    World();
    virtual ~World();

    /* GENERAL */
    void addEntity(he::game::Entity* pEnt);
    void removeEntity(he::game::Entity* pEnt);

    /* GETTERS */
    const std::vector<he::game::Entity*>& getEntities();

private:

    /* DATAMEMBERS */
    std::vector<he::game::Entity*> m_Entities;

    /* DEFAULT COPY & ASSIGNMENT */
    World(const World&);
    World& operator=(const World&);
};

} //end namespace

#endif
