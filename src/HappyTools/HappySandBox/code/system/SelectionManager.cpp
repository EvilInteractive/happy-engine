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
//Author:  
//Created: //

#include "HappySandBoxPCH.h" 

#include "SelectionManager.h"

#include <Entity.h>
#include <EntityManager.h>

namespace hs {

SelectionManger::SelectionManger()
{
}

SelectionManger::~SelectionManger()
{
}

void SelectionManger::deselect( he::ge::Entity* const entity )
{
    size_t index(0);
    HE_ASSERT(entity->getEntityParent() == nullptr, "Not top level entity?");
    if (m_Selection.find(entity->getHandle(), index))
    {
        internalDeselect(entity);
        m_Selection.removeAt(index);
    }
}

void SelectionManger::deselectAll()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_Selection.forEach([this, entityMan](const he::ObjectHandle& handle)
    {
        he::ge::Entity* const entity(entityMan->getEntity(handle));
        internalDeselect(entity);
    });
    m_Selection.clear();
}

void SelectionManger::select( he::ge::Entity* const entity )
{
    if (m_Selection.contains(entity->getHandle()) == false)
    {
        internalSelect(entity);
        m_Selection.add(entity->getHandle());
    }
}

void SelectionManger::internalDeselect( he::ge::Entity* const /*entity*/ )
{
//     EditorComponent* const comp(checked_cast<EditorComponent*>(entity->getComponent(HSFS::strEditorComponent)));
//     if (comp != nullptr)
//     {
//         comp->setSelected(false);
//     }
}

void SelectionManger::internalSelect( he::ge::Entity* const /*entity*/ )
{
//     EditorComponent* const comp(checked_cast<EditorComponent*>(entity->getComponent(HSFS::strEditorComponent)));
//     if (comp != nullptr)
//     {
//         comp->setSelected(true);
//     }
}

} //end namespace