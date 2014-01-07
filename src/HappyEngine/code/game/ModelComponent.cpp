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
#include "EntityComponentDesc.h"
#include "Property.h"
#include "PropertyConverter.h"
#include "PropertyFeel.h"

namespace he {
namespace ge {
    
ModelComponent::ModelComponent()
    : m_ModelMesh(nullptr)
    , m_NewModelMesh(nullptr)
    , m_Parent(nullptr)
    , m_Material(nullptr)
    , m_IsAttached(false)
{
}


ModelComponent::~ModelComponent()
{
    if (isAttachedToScene())
        detachFromScene();
    unloadModelMeshAndMaterial();
}

void ModelComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

const gfx::Material* ModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh* ModelComponent::getModelMesh() const
{
    return m_ModelMesh;
}

void ModelComponent::loadModelMeshAndMaterial( const he::String& materialAsset, const he::String& modelAsset, const he::String& meshName )
{
    he::ct::ContentManager* contentManager(CONTENT);

    ObjectHandle materialHandle(contentManager->loadMaterial(materialAsset));
    gfx::Material* newMaterial(he::ResourceFactory<he::gfx::Material>::getInstance()->get(materialHandle));
    const gfx::BufferLayout& layout(newMaterial->getCompatibleVertexLayout());

    if (m_NewModelMesh != nullptr)
    {
        m_NewModelMesh->cancelLoadCallback(this);
        m_NewModelMesh->release();
    }
    
    m_NewModelMesh = contentManager->asyncLoadModelMesh(modelAsset, meshName, layout);
    m_NewModelMesh->callbackOnceIfLoaded(this, [this, materialAsset](const ELoadResult /*result*/)
    {
        bool reactivate(false);
        if (m_IsAttached)
        {
            deactivate();
            reactivate = true;
        }

        gfx::ModelMesh* const newMesh(m_NewModelMesh);
        m_NewModelMesh = nullptr;

        unloadModelMeshAndMaterial();

        m_ModelMesh = newMesh;

        he::ct::ContentManager* contentManager(CONTENT);
        ObjectHandle materialHandle(contentManager->loadMaterial(materialAsset));
        m_Material = he::ResourceFactory<he::gfx::Material>::getInstance()->get(materialHandle);

        if (reactivate == true)
        {
            activate();
        }

        OnModelMeshLoaded();
    });

    newMaterial->release();
}

void ModelComponent::unloadModelMeshAndMaterial()
{
    HE_ASSERT(m_IsAttached == false && isAttachedToScene() == false, "Trying to unload model while still attached to the scene!");
    if (m_NewModelMesh != nullptr)
    {
        m_NewModelMesh->cancelLoadCallback(this);
        m_NewModelMesh->release();
        m_NewModelMesh = nullptr;
    }
    if (m_ModelMesh != nullptr)
    {
        m_ModelMesh->cancelLoadCallback(this);
        m_ModelMesh->release();
        m_ModelMesh = nullptr;
    }
    if (m_Material != nullptr)
    {
        m_Material->release();
        m_Material = nullptr;
    }
    m_MaterialAsset.clear();
    m_ModelAsset.clear();
}

void ModelComponent::activate()
{
    HE_IF_ASSERT(m_IsAttached == false, "ModelComponent already active")
    HE_IF_ASSERT(m_Parent != nullptr, "Activating ModelComponent without a parent is not possible!")
    {
        m_IsAttached = true;
        if (m_ModelMesh != nullptr && isAttachedToScene() == false)
        {
            attachToScene(m_Parent->getScene());
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
            detachFromScene();
        }
    }
}

void ModelComponent::fillEntityComponentDesc( EntityComponentDesc& desc )
{
    desc.m_ID = HEFS::strModelComponent;
    desc.m_DisplayName = "Model Component";

    Property* modelProp(NEW Property());
    modelProp->init<he::String>(HEFS::strModel, "");
    desc.m_Properties.add(PropertyDesc(modelProp, "Model", "The model to display", 
        NEW PropertyConverterString(), NEW PropertyFeelDefault()));

    Property* materialProp(NEW Property());
    materialProp->init<he::String>(HEFS::strMaterial, "");
    desc.m_Properties.add(PropertyDesc(materialProp, "Material", "The material to use", 
        NEW PropertyConverterString(), NEW PropertyFeelDefault()));
}

bool ModelComponent::setProperty( const Property* const inProperty )
{
    if (EntityComponent::setProperty(inProperty) == false)
    {
        /*const he::FixedString name(inProperty->getName());
        if (name == HEFS::strModel)
        {
            m_ModelAsset = inProperty->get<he::String>();
            if (m_ModelAsset.empty() == false && m_MaterialAsset.empty() == false)
            {
                loadModelMeshAndMaterial(m_MaterialAsset, m_ModelAsset);
            }
            return true;
        }
        else if (name == HEFS::strMaterial)
        {
            m_MaterialAsset = inProperty->get<he::String>();
            if (m_ModelAsset.empty() == false && m_MaterialAsset.empty() == false)
            {
                loadModelMeshAndMaterial(m_MaterialAsset, m_ModelAsset);
            }
            return true;
        }*/
    }
    return false;
}

bool ModelComponent::getProperty( Property* const inOutProperty )
{
    return EntityComponent::getProperty(inOutProperty);
}

} } //end namespace
