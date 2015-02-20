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
#include "EditorPickingManager.h"

#include <ShapeRenderer.h>
#include <Model.h>
#include <ContentManager.h>
#include <ShapeMesh.h>
#include <BinObjLoader.h>
#include <View.h>
#include <ICamera.h>
#include <EntityManager.h>
#include <Ray.h>
#include <Pickable.h>
#include <ControlsManager.h>
#include <Mouse.h>
#include <Window.h>
#include <Plane.h>

namespace hs {

EntityMoveInteractionMode::EntityMoveInteractionMode()
    : m_IsActive(false)
    , m_SelectionChangedCallback(std::bind(&EntityMoveInteractionMode::onSelectionChanged, this))
    , m_OnPick(std::bind(&EntityMoveInteractionMode::onPick, this, std::placeholders::_1, std::placeholders::_2))
    , m_OnHover(std::bind(&EntityMoveInteractionMode::onHover, this, std::placeholders::_1, std::placeholders::_2))
    , m_Size(1.5f)
    , m_Visible(false)
    , m_PickPass(ePickPass_XAxis)
    , m_MoveActive(false)
{
    m_Bound.fromAABB(he::AABB(he::vec3(10, 10, 10), -he::vec3(2, 2, 2)));

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

        hs::EditorPickingManager* pickingManager(hs::EditorPickingManager::getInstance());
        pickingManager->OnPick += m_OnPick;
        pickingManager->OnHover += m_OnHover;

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

        hs::EditorPickingManager* pickingManager(hs::EditorPickingManager::getInstance());
        pickingManager->OnPick -= m_OnPick;
        pickingManager->OnHover -= m_OnHover;

        Sandbox::getInstance()->getRenderPipeline()->getShapeRenderer()->detachFromRenderer(this);

        m_IsActive = false;
    }
}

void EntityMoveInteractionMode::tick( const float /*dTime*/ )
{
    if (m_MoveActive || m_SelectedAxi != 0)
    {
        he::io::IMouse* mouse(CONTROLS->getMouse(Sandbox::getInstance()->getGameWindow()->getHandle()));
        if (mouse->isButtonPressed(he::io::MouseButton_Left))
        {
            m_StartMovePos = getMouseWorldPos(m_SelectedAxi, mouse->getPosition());
            m_LastMovePos = m_StartMovePos;
            m_MoveActive = true;
        }
        else if (mouse->isButtonUp(he::io::MouseButton_Left))
        {
            m_MoveActive = false;
        }

        if (m_MoveActive)
        {
            he::vec3 currentMousePos(getMouseWorldPos(m_SelectedAxi, mouse->getPosition()));
            he::vec3 diff(currentMousePos - m_LastMovePos);
            he::vec3 maskDiff((m_SelectedAxi & BIT(eAxis_X)) ? diff.x : 0, (m_SelectedAxi & BIT(eAxis_Y)) ? diff.y : 0,(m_SelectedAxi & BIT(eAxis_Z)) ? diff.z : 0);
            m_Position += maskDiff;
            m_LastMovePos = currentMousePos;
            he::ge::EntityManager* const entityManager(he::ge::EntityManager::getInstance());
            SelectionManger::getInstance()->getSelection().forEach([entityManager, &maskDiff](const he::ObjectHandle handle)
            {
                he::ge::Entity* entity(entityManager->getEntity(handle));
                if (entity)
                {
                    entity->setLocalTranslate(entity->getLocalTranslate() + maskDiff);
                }
            });
        }
    }
}

bool EntityMoveInteractionMode::onPick( const he::Ray& /*ray*/, he::ge::PickResult& /*result*/ )
{
    return m_MoveActive;
}

bool EntityMoveInteractionMode::onHover( const he::Ray& ray, he::ge::PickResult& result )
{
    bool picked(false);
    if (!m_MoveActive)
    {
        for (size_t i(0); i < ePickPass_MAX; ++i)
        {
            m_PickPass = he::checked_numcast<EPickingPass>(i);
            picked = pick(ray, result);
            if (picked)
                break;
        }
        m_SelectedAxi = 0;
        if (picked)
        {
            switch (m_PickPass)
            {
            case ePickPass_XArrow:
            case ePickPass_XAxis: 
                m_SelectedAxi = BIT(eAxis_X); break;
            case ePickPass_YArrow:
            case ePickPass_YAxis:
                m_SelectedAxi = BIT(eAxis_Y); break;
            case ePickPass_ZArrow:
            case ePickPass_ZAxis:
                m_SelectedAxi = BIT(eAxis_Z); break;
            case ePickPass_XYAxis:
                m_SelectedAxi = BIT(eAxis_X) | BIT(eAxis_Y); break;
            case ePickPass_YZAxis:
                m_SelectedAxi = BIT(eAxis_Y) | BIT(eAxis_Z); break;
            case ePickPass_XZAxis:
                m_SelectedAxi = BIT(eAxis_X) | BIT(eAxis_Z); break;
            default:
                break;
            }
        }
    }
    else
    {
        picked = true;
    }
    return picked;
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

        const bool xSelected(isSelected(eAxis_X));
        const bool ySelected(isSelected(eAxis_Y));
        const bool zSelected(isSelected(eAxis_Z));

        m_AxisWorld[eLookat_X] = he::mat44::createTranslation(m_Position) * he::mat44::createScale(100.0f * scale);
        m_AxisWorld[eLookat_Y] = he::mat44::createTranslation(m_Position) * he::mat44::createRotation(he::vec3::forward, he::piOverTwo) * he::mat44::createScale(100.0f * scale);
        m_AxisWorld[eLookat_Z] = he::mat44::createTranslation(m_Position) * he::mat44::createRotation(he::vec3::up, -he::piOverTwo) * he::mat44::createScale(100.0f * scale);
        m_AxisWorld[eLookat_XZ] = he::mat44::createTranslation(m_Position) * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * he::mat44::createScale(100.0f * scale);

        renderer->drawShape(m_Arrow, m_AxisWorld[eLookat_X], xSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_X]);
        renderer->drawShape(m_Arrow, m_AxisWorld[eLookat_Y], ySelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Y]);
        renderer->drawShape(m_Arrow, m_AxisWorld[eLookat_Z], zSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Z]);
        renderer->drawLine(m_Position, m_Position + he::vec3::right * 8 * scale, xSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_X]);
        renderer->drawLine(m_Position, m_Position + he::vec3::up * 8 * scale, ySelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Y]);
        renderer->drawLine(m_Position, m_Position + he::vec3::forward * 8 * scale, zSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Z]);

        renderer->drawLine(m_Position + he::vec3(3.5f, 0, 0) * scale, m_Position + he::vec3(3.5f, 3.5f, 0) * scale, xSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_X]);
        renderer->drawLine(m_Position + he::vec3(3.5f, 0, 0) * scale, m_Position + he::vec3(3.5f, 0, 3.5f) * scale, xSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_X]);
        
        renderer->drawLine(m_Position + he::vec3(0, 3.5f, 0) * scale, m_Position + he::vec3(3.5f, 3.5f, 0) * scale, ySelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Y]);
        renderer->drawLine(m_Position + he::vec3(0, 3.5f, 0) * scale, m_Position + he::vec3(0, 3.5f, 3.5f) * scale, ySelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Y]);
        
        renderer->drawLine(m_Position + he::vec3(0, 0, 3.5f) * scale, m_Position + he::vec3(0, 3.5f, 3.5f) * scale, zSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Z]);
        renderer->drawLine(m_Position + he::vec3(0, 0, 3.5f) * scale, m_Position + he::vec3(3.5f, 0, 3.5f) * scale, zSelected ? m_SelectedAxisColor : m_AxisColor[eAxis_Z]);
    }
}

bool EntityMoveInteractionMode::getPickingData( const he::vec3*& outVertices, const void*& outIndices, he::gfx::IndexStride& outIndexStride, size_t& outTriangleCount ) const 
{
    he::gfx::PickingData* pickdata(nullptr);
    switch (m_PickPass)
    {
    case ePickPass_XArrow:
    case ePickPass_YArrow:
    case ePickPass_ZArrow:
        pickdata = m_ArrowPicking;
        break;
    case ePickPass_XAxis:
    case ePickPass_YAxis:
    case ePickPass_ZAxis:
        pickdata = m_AxisPicking;
        break;
    case ePickPass_XYAxis:
    case ePickPass_YZAxis:
    case ePickPass_XZAxis:
        pickdata = m_DoubleAxisPicking;
        break;
    }

    if (pickdata)
    {
        outVertices = pickdata->m_Vertices;
        outIndices = pickdata->m_Indices;
        outIndexStride = pickdata->m_IndexStride;
        outTriangleCount = pickdata->m_TriangleCount;
        return true;
    }
    return false;
}

const he::mat44& EntityMoveInteractionMode::getPickingWorld() const 
{
    switch (m_PickPass)
    {
    case ePickPass_XArrow:
    case ePickPass_XAxis:
    case ePickPass_XYAxis:
        return m_AxisWorld[eLookat_X];
    case ePickPass_YArrow:
    case ePickPass_YAxis:
        return m_AxisWorld[eLookat_Y];
    case ePickPass_ZArrow:
    case ePickPass_ZAxis:
    case ePickPass_YZAxis:
        return m_AxisWorld[eLookat_Z];
    case ePickPass_XZAxis:
        return m_AxisWorld[eLookat_XZ];
    }
    return he::mat44::Identity;
}

const he::Bound& EntityMoveInteractionMode::getPickingBound() const 
{
    return m_Bound;
}

he::vec3 EntityMoveInteractionMode::getMouseWorldPos( const he::uint8 axi, const he::vec2& mousePos ) const
{
    bool x(axi & BIT(eAxis_X));
    bool y(axi & BIT(eAxis_Y));
    bool z(axi & BIT(eAxis_Z));

    he::gfx::View* view(Sandbox::getInstance()->getMainView());
    he::Ray ray(view, view->getCamera(), mousePos);

    he::Plane plane;
    if ((x || z) && !y)
        plane = he::Plane(he::vec3::up, -m_Position.y);
    else if (x || (y && abs(dot(he::vec3::forward, ray.getDirection())) > 0.5f))
        plane = he::Plane(he::vec3::forward, -m_Position.z);
    else
        plane = he::Plane(he::vec3::right, -m_Position.x);
    
    float dist(0.0f);
    if (plane.intersect(ray, dist, true))
    {
        return ray.getOrigin() + ray.getDirection() * dist;
    }
    return m_StartMovePos;
}


} //end namespace