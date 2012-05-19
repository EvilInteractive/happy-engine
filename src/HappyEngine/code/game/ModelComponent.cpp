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
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "ModelComponent.h"
#include "Entity.h"
#include "GraphicsEngine.h"
#include "ModelMesh.h"

namespace he {
namespace ge {

ModelComponent::ModelComponent(): m_pModel(nullptr), m_pParent(nullptr)
{
}


ModelComponent::~ModelComponent()
{
    if (m_pModel != nullptr)
        m_pModel->release();
}

void ModelComponent::init(Entity* pParent)
{
    m_pParent = pParent;

    GRAPHICS->addToDrawList(this);
}

void ModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform;
}

void ModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform;
}

const gfx::Material& ModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh* ModelComponent::getModelMesh() const
{
    return m_pModel;
}

mat44 ModelComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

void ModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& ModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void ModelComponent::setModelMesh( const ObjectHandle& modelHandle, bool isPickable )
{
    if (m_pModel != nullptr)
        m_pModel->release();
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(modelHandle);
    m_pModel = ResourceFactory<gfx::ModelMesh>::getInstance()->get(modelHandle);
    setPickable(isPickable);
}

void ModelComponent::setMaterial( const gfx::Material& material )
{
    m_Material = material;
}

} } //end namespace