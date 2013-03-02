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
//Author:  Bastian Damman
//Created: 2013-03-02
#include "HappyPCH.h" 

#include "PickingComponent.h"
#include "Entity.h"
#include "ResourceFactory.h"
#include "ModelMesh.h"
#include "ModelComponent.h"
#include "PickingManager.h"

namespace he {
namespace ge {

IMPLEMENT_ENTITY_COMPONENT_TYPE(PickingComponent)

#pragma warning(disable:4355)
PickingComponent::PickingComponent()
    : m_ModelMesh(nullptr)
    , m_Parent(nullptr)
    , m_OnNewPickingMesh([this](){ initPickingMesh(); })
{
}
#pragma warning(default:4355)


PickingComponent::~PickingComponent()
{
}

void PickingComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

void PickingComponent::activate()
{
    HE_IF_ASSERT(m_Parent != nullptr, "Activating PickingComponent without a parent is not possible!")
    {
        EntityComponent* const comp(m_Parent->getComponent(ModelComponent::s_ComponentType));
        if (comp != nullptr) // Make safer and more eff with objecthandles + keep as member!
        {
            ModelComponent* const modelComp(static_cast<ModelComponent*>(comp));
            modelComp->OnModelMeshLoaded += m_OnNewPickingMesh;
            initPickingMesh();
        }
        else
        {
            LOG(LogType_ArtAssert, "Pickingcomponent could not find a modelcomponent to do the picking with!");
        }
    }
}

void PickingComponent::deactivate()
{
    EntityComponent* const comp(m_Parent->getComponent(ModelComponent::s_ComponentType));
    if (comp != nullptr) // Make safer with objecthandles!
    {
        ModelComponent* const modelComp(static_cast<ModelComponent*>(comp));
        modelComp->OnModelMeshLoaded -= m_OnNewPickingMesh;
    }
    if (m_ModelMesh != nullptr)
    {
        PickingManager::getInstance()->removeFromPickList(this);
        m_ModelMesh->release();
        m_ModelMesh = nullptr;
    }
}


void PickingComponent::getPickingData( const vec3*& outVertices, const void*& outIndices, gfx::IndexStride& outIndexStride, size_t& outTriangleCount ) const
{
    const gfx::PickingData& data(m_ModelMesh->getPickingData());
    outVertices = data.m_Vertices;
    outIndices = data.m_Indices;
    outIndexStride = data.m_IndexStride;
    outTriangleCount = data.m_TriangleCount;
}

const Bound& PickingComponent::getPickingBound() const
{
    return m_ModelMesh->getBound();
}

const mat44& PickingComponent::getPickingWorld() const
{
    EntityComponent* const comp(m_Parent->getComponent(ModelComponent::s_ComponentType));
    if (comp != nullptr) // Make safer with objecthandles!
    {
        return comp->getWorldMatrix();
    }
    return getWorldMatrix();
}

void PickingComponent::initPickingMesh()
{
    EntityComponent* const comp(m_Parent->getComponent(ModelComponent::s_ComponentType));
    if (comp != nullptr) // Make safer with objecthandles!
    {
        ModelComponent* const modelComp(static_cast<ModelComponent*>(comp));
        const gfx::ModelMesh* mesh(modelComp->getModelMesh());
        if (mesh != nullptr)
        {
            if (m_ModelMesh != nullptr)
            {
                m_ModelMesh->release();
            }
            if (m_ModelMesh == nullptr)
            {
                PickingManager::getInstance()->addToPickList(this);
            }
            m_ModelMesh = mesh;
            m_ModelMesh->instantiate();
        }
    }
}

} } //end namespace
