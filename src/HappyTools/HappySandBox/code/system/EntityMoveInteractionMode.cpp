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
#include <BinObjLoader.h>
#include <View.h>
#include <ICamera.h>
#include <EntityManager.h>

namespace hs {

EntityMoveInteractionMode::EntityMoveInteractionMode()
    : m_IsActive(false)
    , m_SelectionChangedCallback(std::bind(&EntityMoveInteractionMode::onSelectionChanged, this))
    , m_Size(1.5f)
    , m_Visible(false)
{
    m_AxisColor[eAxis_X] = he::Color(1.0f, 0.0f, 0.0f);
    m_AxisColor[eAxis_Y] = he::Color(0.0f, 1.0f, 0.0f);
    m_AxisColor[eAxis_Z] = he::Color(0.0f, 0.0f, 1.0f);
    m_SelectedAxisColor = he::Color(1.0f, 1.0f, 0.0f);
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
    he::vec3 pos;
    size_t count(0);
    he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
    SelectionManger::getInstance()->getSelection().forEach([entityManager, &pos, &count](const he::ObjectHandle handle)
    {
        he::ge::Entity* entity(entityManager->getEntity(handle));
        if (entity)
        {
            pos += entity->getLocalTranslate();
            ++count;
        }
    });
    if (count > 0)
    {
        m_Position = pos / count;
        m_Visible = true;
    }
    else
    {
        m_Visible = false;
    }
}

void EntityMoveInteractionMode::createGizmo()
{
    he::ct::models::BinObjLoader loader;
    if (loader.load(CONTENT->getModelFolderPath().append("editor/gizmo.binobj").str()))
    {
        const size_t meshCount(loader.getNumMeshes());
        for (size_t i(0); i < meshCount; ++i)
        {
            const he::String& name(loader.getMeshName(i));
            he::gfx::ShapeMesh* mesh(nullptr);
            he::gfx::PickingData* pick(nullptr);
            if (strcmp(name.c_str(), "m_Translate") == 0)
            {
                m_Arrow = HENew(he::gfx::ShapeMesh)();
                m_ArrowPicking = HENew(he::gfx::PickingData)();
                mesh = m_Arrow;
                pick = m_ArrowPicking;
            }
            else if (strcmp(name.c_str(), "m_Axis") == 0)
            {
                m_AxisPicking = HENew(he::gfx::PickingData)();
                pick = m_AxisPicking;
            }
            else if (strcmp(name.c_str(), "m_TwoAxis") == 0)
            {
                m_DoubleAxisPicking = HENew(he::gfx::PickingData)();
                pick = m_DoubleAxisPicking;
            }
            if (mesh)
            {
                mesh->init(he::gfx::MeshDrawMode_Triangles);
                mesh->beginEditing();
                mesh->addBuffer(loader.getVertices(i), loader.getIndices(i), loader.getNumIndices(i), loader.getIndexStride(i), loader.getVertexLayout());
                mesh->endEditing(false, false);
            }
            if (pick)
            {
                pick->createPickingData(loader.getVertices(i), loader.getNumVertices(i), loader.getVertexLayout(), loader.getIndices(i), loader.getNumIndices(i), loader.getIndexStride(i));
            }
        }
    }
}

void EntityMoveInteractionMode::destroyGizmo()
{
    HEDelete(m_Arrow);
    m_Arrow = nullptr;
    HEDelete(m_ArrowPicking);
    m_ArrowPicking = nullptr;
    HEDelete(m_AxisPicking);
    m_AxisPicking = nullptr;
    HEDelete(m_DoubleAxisPicking);
    m_DoubleAxisPicking = nullptr;
}

void EntityMoveInteractionMode::drawShapes( he::gfx::ShapeRenderer* const renderer )
{
    if (m_Visible)
    {
        he::gfx::ICamera* camera(renderer->getView()->getCamera());
        const float dist(he::length(camera->getPosition() - m_Position));
        const float scale(dist / 100 * m_Size);

        renderer->drawShape(m_Arrow, he::mat44::createTranslation(m_Position) * he::mat44::createScale(100.0f * scale), m_AxisColor[eAxis_X]);
        renderer->drawShape(m_Arrow, he::mat44::createTranslation(m_Position) * he::mat44::createRotation(he::vec3::forward, he::piOverTwo) * he::mat44::createScale(100.0f * scale), m_AxisColor[eAxis_Y]);
        renderer->drawShape(m_Arrow, he::mat44::createTranslation(m_Position) * he::mat44::createRotation(he::vec3::up, -he::piOverTwo) * he::mat44::createScale(100.0f * scale), m_AxisColor[eAxis_Z]);
        renderer->drawLine(m_Position, m_Position + he::vec3::right * 8 * scale, m_AxisColor[eAxis_X]);
        renderer->drawLine(m_Position, m_Position + he::vec3::up * 8 * scale, m_AxisColor[eAxis_Y]);
        renderer->drawLine(m_Position, m_Position + he::vec3::forward * 8 * scale, m_AxisColor[eAxis_Z]);

        renderer->drawLine(m_Position + he::vec3(4, 0, 0) * scale, m_Position + he::vec3(4, 4, 0) * scale, m_AxisColor[eAxis_X]);
        renderer->drawLine(m_Position + he::vec3(4, 0, 0) * scale, m_Position + he::vec3(4, 0, 4) * scale, m_AxisColor[eAxis_X]);
        
        renderer->drawLine(m_Position + he::vec3(0, 4, 0) * scale, m_Position + he::vec3(4, 4, 0) * scale, m_AxisColor[eAxis_Y]);
        renderer->drawLine(m_Position + he::vec3(0, 4, 0) * scale, m_Position + he::vec3(0, 4, 4) * scale, m_AxisColor[eAxis_Y]);
        
        renderer->drawLine(m_Position + he::vec3(0, 0, 4) * scale, m_Position + he::vec3(0, 4, 4) * scale, m_AxisColor[eAxis_Z]);
        renderer->drawLine(m_Position + he::vec3(0, 0, 4) * scale, m_Position + he::vec3(4, 0, 4) * scale, m_AxisColor[eAxis_Z]);
    }
}

} //end namespace