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
//Created: 19/01/2012

#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_
#pragma once

#include "HappyTypes.h"
#include "Entity.h"
#include <vector>

namespace happysandbox {

class EntityManager
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    EntityManager();
    virtual ~EntityManager();

    /* GENERAL */
    he::uint addEntity(he::game::Entity* pEntity);
    void deleteEntity(he::uint id);

    /* GETTERS */
    he::game::Entity* getEntity(he::uint id) const;
    const std::vector<he::game::Entity*>& getEntityList() const;

private:

    /* DATAMEMBERS */
    std::vector<he::game::Entity*> m_EntityList;

    /* DEFAULT COPY & ASSIGNMENT */
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};

} //end namespace

#endif
