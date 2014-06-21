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
//Author:  Bastian Damman
//Created: 2013/04/13

#include "HappySandBoxPCH.h" 

#include "EditorComponent.h"
#include "system/EditorPickingManager.h"
#include "system/SandboxRenderPipeline.h"
#include "system/SelectionManager.h"
#include "Sandbox.h"

#include <EntityManager.h>
#include <PickingComponent.h>
#include <ModelComponent.h>
#include <ShapeRenderer.h>

namespace hs {

EditorComponent::EditorComponent()
    : m_Parent(nullptr)
    , m_PickingComponent(nullptr)
    , m_IsSelected(false)
{
}

EditorComponent::~EditorComponent()
{
    HE_ASSERT(m_IsSelected == false, "Destroying still selected entity!");
}

void EditorComponent::init( he::ge::Entity* parent )
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;

    he::ge::PickingComponent* const pickingComp(he::checked_cast<he::ge::PickingComponent*>(
        he::ge::EntityManager::getInstance()->createComponent(he::HEFS::strPickingComponent)));
    pickingComp->setPickingManager(EditorPickingManager::getInstance());
    m_Parent->addComponent(pickingComp);
}

void EditorComponent::activate()
{
    computeBound();
}

void EditorComponent::deactivate()
{
}

void EditorComponent::computeBound()
{
    bool first(true);

    he::ge::Entity* const entity(getEntityParent());
    const size_t compCount(entity->getComponentCount());
    for (size_t i(0); i < compCount; ++i)
    {
        he::ge::EntityComponent* const comp(entity->getComponentAt(i));
        if (comp->getComponentID() == he::HEFS::strModelComponent)
        {
            he::ge::ModelComponent* const modelComp(he::checked_cast<he::ge::ModelComponent*>(comp));
            if (first)
            {
                m_AABB = modelComp->getBound().getAABB();
                first = false;
            }
            else
            {
                m_AABB.merge(modelComp->getBound().getAABB());
            }
        }
    }
}

void EditorComponent::setSelected( const bool selected )
{
    if (m_IsSelected != selected)
    {
        m_IsSelected = selected;
        he::gfx::ShapeRenderer* const shapeRenderer(Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer());
        if (m_IsSelected == true)
        {
            computeBound();
            shapeRenderer->attachToRenderer(this);
        }
        else
        {
            shapeRenderer->detachFromRenderer(this);       
        }
    }
}

void EditorComponent::drawShapes( he::gfx::ShapeRenderer* const renderer )
{
    const he::vec3 center((m_AABB.getBottomBackRight() + m_AABB.getTopFrontLeft()) / 2.0f);
    const he::vec3 dim(m_AABB.getBottomBackRight() - m_AABB.getTopFrontLeft());
    renderer->drawAABB(center, dim, SelectionManger::s_SelectionColor);
}

} //end namespace
