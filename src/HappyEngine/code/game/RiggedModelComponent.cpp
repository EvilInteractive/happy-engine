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
#include "StdAfx.h" 

#include "RiggedModelComponent.h"
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"

namespace he {
namespace game {

RiggedModelComponent::RiggedModelComponent()
{
}


RiggedModelComponent::~RiggedModelComponent()
{
}

void RiggedModelComponent::init( Entity* pParent )
{
    m_pParent = pParent;
    GRAPHICS->addToDrawList(this);
}

void RiggedModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform << m_CastShadow;
}

void RiggedModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform >> m_CastShadow;
}

const gfx::Material& RiggedModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh::pointer& RiggedModelComponent::getModel() const
{
    return m_pModel;
}

mat44 RiggedModelComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

bool RiggedModelComponent::getCastsShadow() const
{
    return m_CastShadow;
}

void RiggedModelComponent::setCastsShadow( bool casts )
{
    m_CastShadow = casts;
}

void RiggedModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& RiggedModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void RiggedModelComponent::setModel( const gfx::ModelMesh::pointer& pModel )
{
    m_pModel = pModel;
    m_BoneTransform.clear();
    m_Bones.clear();
    std::for_each(pModel->getBones().cbegin(), pModel->getBones().cend(), [&](const gfx::Bone& bone)
    {
        m_BoneTransform.push_back(bone.m_BaseTransform);
        m_Bones[bone.m_Name] = &m_BoneTransform.back();
    });
}

void RiggedModelComponent::setMaterial( const gfx::Material& material )
{
    m_Material = material;
}

const std::vector<mat44>& RiggedModelComponent::getBoneTransforms() const
{
    return m_BoneTransform;
}

} } //end namespace