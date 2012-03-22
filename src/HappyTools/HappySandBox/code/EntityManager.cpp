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

#include "EntityManager.h"
#include "HappyPCH.h"

using namespace he;

namespace happysandbox {

/* CONSTRUCTOR - DESTRUCTOR */
EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
    std::for_each(m_EntityList.begin(), m_EntityList.end(), [](he::game::Entity* pEntity)
    {
        delete pEntity;
        pEntity = nullptr;
    });
}

/* GENERAL */
he::uint EntityManager::addEntity(he::game::Entity* pEntity)
{
    m_EntityList.push_back(pEntity);

    return m_EntityList.size() - 1;
}

void EntityManager::deleteEntity(he::uint id)
{
    he::game::Entity* pEntity(m_EntityList[id]);

    delete pEntity;
    pEntity = nullptr;

    m_EntityList.erase(std::remove(m_EntityList.begin(), m_EntityList.end(), pEntity), m_EntityList.end());
}

/* GETTERS */
he::game::Entity* EntityManager::getEntity(he::uint id) const
{
    return m_EntityList[id];
}

const std::vector<he::game::Entity*>& EntityManager::getEntityList() const
{
    return m_EntityList;
}

} //end namespace