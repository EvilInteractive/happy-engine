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
//Created: 2015/02/10
#include "HappySandBoxPCH.h" 
#include "EntityMoveInteractionMode.h"

#include "Sandbox.h"
#include "SelectionManager.h"
#include "SandboxRenderPipeline.h"

#include <ShapeRenderer.h>
#include <Model.h>
#include <ContentManager.h>
#include <ShapeMesh.h>

namespace hs {

EntityMoveInteractionMode::EntityMoveInteractionMode()
    : m_IsActive(false)
    , m_SelectionChangedCallback(std::bind(&EntityMoveInteractionMode::onSelectionChanged, this))
{
    createGizmo();
}

EntityMoveInteractionMode::~EntityMoveInteractionMode()
{
    destroyGizmo();
}

void EntityMoveInteractionMode::activate()
{
    if (!m_IsActive)
    {
        SelectionManger* selectionManager(SelectionManger::getInstance());
        selectionManager->SelectionChanged += m_SelectionChangedCallback;

        Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer()->attachToRenderer(this);

        onSelectionChanged();
        m_IsActive = true;
    }
}

void EntityMoveInteractionMode::deactivate()
{
    if (m_IsActive)
    {
        SelectionManger* selectionManager(SelectionManger::getInstance());
        selectionManager->SelectionChanged -= m_SelectionChangedCallback;

        Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer()->detachFromRenderer(this);

        m_IsActive = false;
    }
}

void EntityMoveInteractionMode::tick( const float /*dTime*/ )
{
}

void EntityMoveInteractionMode::onSelectionChanged()
{

}

void EntityMoveInteractionMode::createGizmo()
{
}

void EntityMoveInteractionMode::destroyGizmo()
{

}

void EntityMoveInteractionMode::drawShapes( he::gfx::ShapeRenderer* const /*renderer*/ )
{
}

} //end namespace