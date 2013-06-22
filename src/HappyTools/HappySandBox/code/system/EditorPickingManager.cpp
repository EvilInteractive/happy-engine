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
//Author:  Damman Bastian
//Created: 2013/04/13

#include "HappySandBoxPCH.h" 
#include "EditorPickingManager.h"
#include "Pickable.h"

namespace hs {

EditorPickingManager::EditorPickingManager()
{

}

EditorPickingManager::~EditorPickingManager()
{
    HE_ASSERT(m_PickList.empty(), "Picklist leak!, %d objects still attached", m_PickList.size());
}

void EditorPickingManager::addToPickList( he::ge::Pickable* const object )
{
    HE_IF_ASSERT(m_PickList.contains(object) == false, "Picklist already contains object!")
    {
        m_PickList.add(object);
    }
}

void EditorPickingManager::removeFromPickList( he::ge::Pickable* const object )
{
    HE_IF_ASSERT(m_PickList.contains(object) == true, "Picklist does not contain object!")
    {
        m_PickList.remove(object);
    }
}

bool EditorPickingManager::pick( const he::Ray& ray, he::ge::PickResult& result )
{
    bool hasPicked(false);
    m_PickList.forEach([&hasPicked, &ray, &result](he::ge::Pickable* const pickable)
    {
        hasPicked |= pickable->pick(ray, result);
    });
    return hasPicked;
}


} //end namespace