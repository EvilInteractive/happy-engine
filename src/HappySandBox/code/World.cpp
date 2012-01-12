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

#include "World.h"
#include "HappyPCH.h"

namespace happysandbox {

/* CONSTRUCTOR - DESTRUCTOR */
World::World()
{
}

World::~World()
{
}

/* GENERAL */
void World::addEntity(he::game::Entity* pEnt)
{
    m_Entities.push_back(pEnt);
}

void World::removeEntity(he::game::Entity* pEnt)
{
    m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), pEnt), m_Entities.end());
}

/* GETTERS */
const std::vector<he::game::Entity*>& World::getEntities()
{
    return m_Entities;
}

} //end namespace