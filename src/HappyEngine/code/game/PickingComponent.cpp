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
//Created: 2013-03-02
#include "HappyPCH.h" 

#include "PickingComponent.h"
#include "Entity.h"
#include "ResourceFactory.h"
#include "ModelMesh.h"
#include "ModelComponent.h"
#include "IPickingManager.h"
#include "EntityManager.h"
#include "EntityComponentDesc.h"

namespace he {
namespace ge {
    
#pragma warning(disable:4355)
PickingComponent::PickingComponent()
    : m_ModelMesh(nullptr)
    , m_Parent(nullptr)
    , m_OnNewPickingMesh([this](){ initPickingMesh(); })
    , m_PickingManager(nullptr)
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
        HE_ASSERT(m_PickingManager != nullptr, "You did not set a PickingManager on this pickingcomponent!");
        EntityComponent* const comp(m_Parent->getComponent(HEFS::strModelComponent));
        if (comp != nullptr)
        {
            ModelComponent* const modelComp(checked_cast<ModelComponent*>(comp));
            modelComp->OnModelMeshLoaded += m_OnNewPickingMesh;
            m_LinkedModelComponent = modelComp->getHandle();
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
    if (m_LinkedModelComponent != ObjectHandle::unassigned)
    {
        EntityComponent* const comp(EntityManager::getInstance()->getComponent(m_LinkedModelComponent));
        ModelComponent* const modelComp(checked_cast<ModelComponent*>(comp));
        modelComp->OnModelMeshLoaded -= m_OnNewPickingMesh;
    }
    if (m_ModelMesh != nullptr)
    {
        m_PickingManager->removeFromPickList(this);
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
    if (m_LinkedModelComponent != ObjectHandle::unassigned)
    {
        EntityComponent* const comp(EntityManager::getInstance()->getComponent(m_LinkedModelComponent));
        return comp->getWorldMatrix();
    }
    return getWorldMatrix();
}

void PickingComponent::initPickingMesh()
{
    if (m_LinkedModelComponent != ObjectHandle::unassigned)
    {
        EntityComponent* const comp(EntityManager::getInstance()->getComponent(m_LinkedModelComponent));
        ModelComponent* const modelComp(checked_cast<ModelComponent*>(comp));
        const gfx::ModelMesh* mesh(modelComp->getModelMesh());
        if (mesh != nullptr)
        {
            if (m_ModelMesh != nullptr)
            {
                m_ModelMesh->release();
            }
            if (m_ModelMesh == nullptr)
            {
                m_PickingManager->addToPickList(this);
            }
            m_ModelMesh = mesh;
            m_ModelMesh->instantiate();
        }
    }
}

void PickingComponent::fillEntityComponentDesc( EntityComponentDesc& /*desc*/ )
{
    LOG(LogType_ProgrammerAssert, "Not implemented");
}

bool PickingComponent::setProperty( const Property* const inProperty )
{
    return EntityComponent::setProperty(inProperty);
}

bool PickingComponent::getProperty( Property* const inOutProperty )
{
    return EntityComponent::getProperty(inOutProperty);
}

void PickingComponent::setPickingManager( he::ge::IPickingManager* const manager )
{
    m_PickingManager = manager;
}

} } //end namespace
