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
//Author:  
//Created: //

#include "HappySandBoxPCH.h" 

#include "SelectionManager.h"
#include "Sandbox.h"
#include "SandboxRenderPipeline.h"
#include "runtime/EditorComponent.h"

#include <Entity.h>
#include <EntityManager.h>
#include <ModelComponent.h>
#include <ShapeRenderer.h>

namespace hs {

const he::Color SelectionManger::s_SelectionColor(1.0f, 0.3f, 0.01f, 1.0f);

SelectionManger::SelectionManger()
{
    he::eventCallback0<void> selectionChangedCallback( [this]() { recomputeBoundingBox(); } );
    SelectionChanged += selectionChangedCallback;
}

SelectionManger::~SelectionManger()
{
}

void SelectionManger::init()
{
    Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer()->attachToRenderer(this);
}

void SelectionManger::destroy()
{
    Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer()->detachFromRenderer(this);
}
void SelectionManger::deselect( he::ge::Entity* const entity )
{
    size_t index(0);
    HE_ASSERT(entity->getEntityParent() == nullptr, "Not top level entity?");
    if (m_Selection.find(entity->getHandle(), index))
    {
        if (m_HoverEntity != entity->getHandle())
            internalDeselect(entity);
        m_Selection.removeAt(index);        
        SelectionChanged();
    }
}

void SelectionManger::deselectAll()
{
    he::ge::EntityManager* const entityMan(he::ge::EntityManager::getInstance());
    m_Selection.forEach([this, entityMan](const he::ObjectHandle& handle)
    {
        he::ge::Entity* const entity(entityMan->getEntity(handle));
        if (m_HoverEntity != entity->getHandle())
            internalDeselect(entity);
    });
    const bool hadItems(m_Selection.size());
    m_Selection.clear();
    if (hadItems)
        SelectionChanged();
}

void SelectionManger::select( he::ge::Entity* const entity )
{
    if (m_Selection.contains(entity->getHandle()) == false)
    {
        if (m_HoverEntity != entity->getHandle())
            internalSelect(entity);
        m_Selection.add(entity->getHandle());
        SelectionChanged();
    }
}

void SelectionManger::internalDeselect( he::ge::Entity* const entity )
{
    EditorComponent* const comp(he::checked_cast<EditorComponent*>(entity->getComponent(HSFS::strEditorComponent)));
    if (comp != nullptr)
    {
        comp->setSelected(false);
    }
}

void SelectionManger::internalSelect( he::ge::Entity* const entity )
{
    EditorComponent* const comp(he::checked_cast<EditorComponent*>(entity->getComponent(HSFS::strEditorComponent)));
    if (comp != nullptr)
    {
        comp->setSelected(true);
    }
}

void SelectionManger::drawShapes( he::gfx::ShapeRenderer* const renderer )
{
    if (m_Selection.empty() == false)
    {
        const he::vec3 center((m_AABB.getBottomBackRight() + m_AABB.getTopFrontLeft()) / 2.0f);
        const he::vec3 dim(m_AABB.getBottomBackRight() - m_AABB.getTopFrontLeft());
        renderer->drawAABB(center, dim, s_SelectionColor);
    }
}

void SelectionManger::recomputeBoundingBox()
{
    he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
    bool first(true);
    m_Selection.forEach([this, entityManager, &first](const he::ObjectHandle& entityHandle)
    {
        he::ge::Entity* const entity(entityManager->getEntity(entityHandle));
        EditorComponent* const editorComp(he::checked_cast<EditorComponent*>(entity->getComponent(HSFS::strEditorComponent)));
        if (first)
        {
            m_AABB = editorComp->getBound();
            first = false;
        }
        else
        {
            m_AABB.merge(editorComp->getBound());
        }
    });
}


} //end namespace
