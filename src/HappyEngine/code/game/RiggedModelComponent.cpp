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

#include "RiggedModelComponent.h"
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Console.h"

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
    setVisible(false);
    GRAPHICS->addToDrawList(this);
}

void RiggedModelComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform;
}

void RiggedModelComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform;
}

const gfx::Material& RiggedModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh::pointer& RiggedModelComponent::getModelMesh() const
{
    return m_pModel;
}

mat44 RiggedModelComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

void RiggedModelComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& RiggedModelComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void RiggedModelComponent::setModelMesh( const gfx::ModelMesh::pointer& pModel )
{
    m_pModel = pModel;
    m_BoneTransform.clear();
    m_Bones.clear();

    pModel->callbackIfLoaded(boost::bind(&RiggedModelComponent::modelLoadedCallback, this, pModel));
}
void RiggedModelComponent::modelLoadedCallback( const gfx::ModelMesh::pointer& pMesh )
{
    if (pMesh == m_pModel) //Mesh could've changed
    {
        m_BoneTransform.reserve(pMesh->getBones().size());
        std::for_each(pMesh->getBones().cbegin(), pMesh->getBones().cend(), [&](const gfx::Bone& bone)
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
            setVisible(true);
            onModelLoaded();
        }
        else
            CONSOLE->addMessage("SkinnedMesh error: no bones found in " + pMesh->getName(), CMSG_TYPE_ERROR);
    }
}


void RiggedModelComponent::setMaterial( const gfx::Material& material )
{
    m_Material = material;
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
        CONSOLE->addMessage("RiggedModelComponent error: No bone exists with name: " + name, CMSG_TYPE_ERROR);
        return BoneTransform();
    }
    return m_Bones.at(name);
}


} } //end namespace