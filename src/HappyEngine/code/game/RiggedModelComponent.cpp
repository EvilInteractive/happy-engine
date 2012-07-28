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

#include "RiggedModelComponent.h"

#include "Entity.h"
#include "GraphicsEngine.h"
#include "ModelMesh.h"
#include "Material.h"

namespace he {
namespace ge {

RiggedModelComponent::RiggedModelComponent(): m_Model(nullptr), m_Parent(nullptr), m_Material(nullptr)
{
}


RiggedModelComponent::~RiggedModelComponent()
{
    if (m_Model != nullptr)
        m_Model->release();
    if (m_Material != nullptr)
        m_Material->release();
}

void RiggedModelComponent::init( Entity* parent )
{
    m_Parent = parent;
}

void RiggedModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform;
}

void RiggedModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform;
}

const gfx::Material* RiggedModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh* RiggedModelComponent::getModelMesh() const
{
    return m_Model;
}

mat44 RiggedModelComponent::getWorldMatrix() const
{
    return m_Parent->getWorldMatrix() * m_mtxLocalTransform;
}

void RiggedModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& RiggedModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void RiggedModelComponent::setModelMesh( const ObjectHandle& modelHandle )
{
    if (m_Model != nullptr)
        m_Model->release();
    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(modelHandle);
    m_Model = ResourceFactory<gfx::ModelMesh>::getInstance()->get(modelHandle);
    m_BoneTransform.clear();
    m_Bones.clear();

    ResourceFactory<gfx::ModelMesh>::getInstance()->get(modelHandle)->callbackOnceIfLoaded(boost::bind(&RiggedModelComponent::modelLoadedCallback, this));
}
void RiggedModelComponent::modelLoadedCallback()
{
    m_BoneTransform.reserve(m_Model->getBones().size());
    std::for_each(m_Model->getBones().cbegin(), m_Model->getBones().cend(), [&](const gfx::Bone& bone)
    {
        //m_BoneTransform.push_back(bone.m_BaseTransform);
        m_BoneTransform.push_back(mat44::Identity);

        BoneTransform transform;
        transform.m_ToOrigTransform = bone.m_BaseTransform;
        transform.m_FromOrigTransform = bone.m_BaseTransform.inverse();
        transform.m_RealTransform = &m_BoneTransform.back();

        m_Bones[bone.m_Name] = transform;
    });

    if (m_BoneTransform.size() > 0)
    {

        onModelLoaded();
    }
    else
    {
        HE_ERROR("SkinnedMesh error: no bones found in %s", m_Model->getName());
    }
}


void RiggedModelComponent::setMaterial( const ObjectHandle& material )
{
    if (m_Material != nullptr)
        m_Material->release();
    m_Material = ResourceFactory<gfx::Material>::getInstance()->get(material);
    if (m_Material != nullptr)
        ResourceFactory<gfx::Material>::getInstance()->instantiate(material);
}

const std::vector<mat44>& RiggedModelComponent::getBoneTransforms() const
{
    return m_BoneTransform;
}

RiggedModelComponent::BoneTransform RiggedModelComponent::getBone( const std::string& name ) const
{
    std::map<std::string, BoneTransform>::const_iterator it(m_Bones.find(name));
    if (it == m_Bones.cend())
    {
        HE_ERROR("RiggedModelComponent error: No bone exists with name: %s", name.c_str());
        return BoneTransform();
    }
    else
    {
        return it->second;
    }
}


} } //end namespace