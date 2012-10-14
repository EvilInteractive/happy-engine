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

#include "ContentManager.h"
#include "Model.h"

namespace he {
namespace ge {

ModelComponent::ModelComponent(): m_ModelMesh(nullptr), m_Parent(nullptr), m_AttachedToScene(false), m_Material(nullptr)
{
}


ModelComponent::~ModelComponent()
{
    if (m_ModelMesh != nullptr)
        m_ModelMesh->release();
    if (m_AttachedToScene)
        GRAPHICS->removeFromDrawList(this);
    if (m_Material != nullptr)
        m_Material->release();
}

void ModelComponent::init(Entity* parent)
{
    m_Parent = parent;

    if (m_ModelMesh != nullptr && m_AttachedToScene == false)
    {
        GRAPHICS->addToDrawList(this);
        m_AttachedToScene = true;
    }
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
        if (meshName == "")
            m_ModelMesh = model->instantiateMesh(0);
        else
            m_ModelMesh = model->instantiateMesh(meshName);

        model->release();

        if (m_AttachedToScene == false && m_Parent != nullptr)
        {
            GRAPHICS->addToDrawList(this);
            m_AttachedToScene = true;
        }
    });
}


} } //end namespace