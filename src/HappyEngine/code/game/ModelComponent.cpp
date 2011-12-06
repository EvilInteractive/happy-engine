//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"

namespace he {
namespace game {

ModelComponent::ModelComponent()
{
}


ModelComponent::~ModelComponent()
{
}

void ModelComponent::init( Entity* pParent )
{
    m_pParent = pParent;
    GRAPHICS->addToDrawList(this);
}

void ModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform << m_CastShadow;
}

void ModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform >> m_CastShadow;
}

const gfx::Material& ModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh::pointer& ModelComponent::getModel() const
{
    return m_pModel;
}

mat44 ModelComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

bool ModelComponent::getCastsShadow() const
{
    return m_CastShadow;
}

void ModelComponent::setCastsShadow( bool casts )
{
    m_CastShadow = casts;
}

void ModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& ModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void ModelComponent::setModel( const gfx::ModelMesh::pointer& pModel, bool isPickable )
{
    m_pModel = pModel;
	setPickable(isPickable);
}

void ModelComponent::setMaterial( const gfx::Material& material )
{
    m_Material = material;
}

} } //end namespace