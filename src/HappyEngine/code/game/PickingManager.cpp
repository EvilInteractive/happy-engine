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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#include "HappyPCH.h" 
#include "PickingManager.h"
#include "Pickable.h"

namespace he {
namespace ge {

PickingManager::PickingManager()
{

}

PickingManager::~PickingManager()
{
    HE_ASSERT(m_PickList.empty(), "Picklist leak!, %d objects still attached", m_PickList.size());
}

void PickingManager::addToPickList( Pickable* const object )
{
    HE_IF_ASSERT(m_PickList.contains(object) == false, "Picklist already contains object!")
    {
        m_PickList.add(object);
    }
}

void PickingManager::removeFromPickList( Pickable* const object )
{
    HE_IF_ASSERT(m_PickList.contains(object) == true, "Picklist does not contain object!")
    {
        m_PickList.remove(object);
    }
}

bool PickingManager::pick( const Ray& ray, PickResult& result )
{
    bool hasPicked(false);
    m_PickList.forEach([&hasPicked, &ray, &result](Pickable* const pickable)
    {
        hasPicked |= pickable->pick(ray, result);
    });
    return hasPicked;
}


} } //end namespace