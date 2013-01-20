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
#include "Material.h"
#include "Scene.h"

#include "ContentManager.h"
#include "Model.h"

namespace he {
namespace ge {

ModelComponent::ModelComponent()
    : m_ModelMesh(nullptr)
    , m_Parent(nullptr)
    , m_Material(nullptr)
    , m_IsAttached(false)
{
}


ModelComponent::~ModelComponent()
{
    if (m_ModelMesh != nullptr)
        m_ModelMesh->release();
    if (isAttachedToScene())
        detachFromScene();
    if (m_Material != nullptr)
        m_Material->release();
}

void ModelComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

void ModelComponent::serialize(SerializerStream& /*stream*/)
{
    //Object3D::serialize(stream);
}

void ModelComponent::deserialize(const SerializerStream& /*stream*/)
{
    //Object3D::deserialize(stream);
}

const gfx::Material* ModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh* ModelComponent::getModelMesh() const
{
    return m_ModelMesh;
}

void ModelComponent::setModelMeshAndMaterial( const std::string& materialAsset, const std::string& modelAsset, const std::string& meshName )
{
    he::ct::ContentManager* contentManager(CONTENT);

    ObjectHandle materialHandle(contentManager->loadMaterial(materialAsset));
    m_Material = he::ResourceFactory<he::gfx::Material>::getInstance()->get(materialHandle);
    const gfx::BufferLayout& layout(m_Material->getCompatibleVertexLayout());

    he::gfx::Model* model(contentManager->asyncLoadModel(modelAsset, layout));
    model->callbackOnceIfLoaded([&, model, meshName]()
    {
        bool reactivate(false);
        if (m_IsAttached)
        {
            deactivate();
            reactivate = true;
        }
        if (m_ModelMesh != nullptr)
        {
            m_ModelMesh->release();
            m_ModelMesh = nullptr;
        }

        if (meshName == "")
            m_ModelMesh = model->instantiateMesh(0);
        else
            m_ModelMesh = model->instantiateMesh(meshName);

        model->release();

        if (reactivate == true)
        {
            activate();
        }
    });
}

void ModelComponent::activate()
{
    HE_IF_ASSERT(m_IsAttached == false, "ModelComponent already active")
    HE_IF_ASSERT(m_Parent != nullptr, "Activating ModelComponent without a parent is not possible!")
    {
        m_IsAttached = true;
        if (m_ModelMesh != nullptr)
        {
            m_ModelMesh->callbackOnceIfLoaded([this]()
            {
                if (m_IsAttached = true && isAttachedToScene() == false)
                    m_Parent->getScene()->attachToScene(this);
            });
        }
    }
}

void ModelComponent::deactivate()
{
    HE_IF_ASSERT(m_IsAttached == true, "ModelComponent not active")
    {
        m_IsAttached = false;
        if (isAttachedToScene())
        {
            m_Parent->getScene()->detachFromScene(this);
        }
    }
}

} } //end namespace
