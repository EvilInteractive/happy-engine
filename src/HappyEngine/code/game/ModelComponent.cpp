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
#include "MaterialInstance.h"

namespace he {
namespace ge {
    
ModelComponent::ModelComponent()
    : m_Parent(nullptr)
    , m_Drawable(NEW gfx::Drawable())
    , m_LoadingDesc(nullptr)
{
}


ModelComponent::~ModelComponent()
{
    if (m_Drawable->isAttachedToScene())
        m_Drawable->detachFromScene();
    delete m_Drawable;
}

void ModelComponent::init(Entity* parent)
{
    HE_ASSERT(parent != nullptr, "Parent can not be nullptr! - fatal crash");
    m_Parent = parent;
}

void ModelComponent::loadModelMeshAndMaterial( const he::String& materialAsset, const he::String& modelAsset, const he::String& meshName )
{
    he::ct::ContentManager* const contentManager(CONTENT);

    if (m_LoadingDesc != nullptr)
    {
        gfx::Material* const material(m_LoadingDesc->m_Material);
        if (material != nullptr)
        {
            material->release();
        }
        gfx::ModelMesh* const mesh(m_LoadingDesc->m_Mesh);
        if (mesh != nullptr)
        {
            mesh->cancelLoadCallback(this);
            mesh->release();
        }
    }
    else
    {
        m_LoadingDesc = NEW LoadDesc();
    }

    he::gfx::Material* const material(contentManager->loadMaterial(materialAsset));
    m_LoadingDesc->m_Material = material;

    he::gfx::ModelMesh* const mesh(contentManager->asyncLoadModelMesh(modelAsset, meshName));
    m_LoadingDesc->m_Mesh = mesh;

    // Mesh will probably take the longest
    mesh->callbackOnceIfLoaded(this, std::bind(&ModelComponent::onLoadingDone, this, std::placeholders::_1));
}

void ModelComponent::onLoadingDone( const ELoadResult result )
{
    if (result == eLoadResult_Success && !m_LoadingDesc->m_Material->isLoaded())
    {
        // If the material would take longer to load
        m_LoadingDesc->m_Material->callbackOnceIfLoaded(this, std::bind(&ModelComponent::onLoadingDone, this, std::placeholders::_1));
    }
    else
    {
        // If loading fails, clear the mesh and material
        // This is always done because it is more efficient to reset the drawable first
        m_Drawable->setMaterial(nullptr);
        m_Drawable->setModelMesh(nullptr);
        if (result == eLoadResult_Success)
        {
            m_Drawable->setModelMesh(m_LoadingDesc->m_Mesh);
            m_Drawable->setMaterial(m_LoadingDesc->m_Material);
            if (m_Parent->isActive() && !m_Drawable->isAttachedToScene())
            {
                m_Drawable->attachToScene(m_Parent->getScene());
            }
        }
        m_LoadingDesc->m_Mesh->release();
        m_LoadingDesc->m_Material->release();
        delete m_LoadingDesc;
        m_LoadingDesc = nullptr;
    }
}

void ModelComponent::unloadModelMeshAndMaterial()
{
    HE_ASSERT(m_Drawable->isAttachedToScene() == false, "Trying to unload model while still attached to the scene!");
    m_Drawable->setModelMesh(nullptr);
    m_Drawable->setMaterial(nullptr);
}

void ModelComponent::activate()
{
    HE_IF_ASSERT(m_Drawable->isAttachedToScene() == false, "ModelComponent already active")
    HE_IF_ASSERT(m_Parent != nullptr, "Activating ModelComponent without a parent is not possible!")
    if (m_Drawable->getModelMesh() && m_Drawable->getModelMesh()->isLoaded() && 
        m_Drawable->getMaterial() && m_Drawable->getMaterial()->isLoaded())
    {
        m_Drawable->attachToScene(m_Parent->getScene());
    }
}

void ModelComponent::deactivate()
{
    HE_IF_ASSERT( m_Drawable->isAttachedToScene() == true, "ModelComponent not active")
    {
        m_Drawable->detachFromScene();
    }
}

void ModelComponent::fillEntityComponentDesc( EntityComponentDesc& desc )
{
    desc.m_ID = HEFS::strModelComponent;
    desc.m_DisplayName = "Model Component";

    EntityComponent::fillEntityComponentDesc(desc);

    Property* modelProp(NEW Property());
    modelProp->init<he::String>(HEFS::strModel, "");
    desc.m_Properties.setAt(modelProp->getName(), PropertyDesc(modelProp, "Model", "The model to display", 
        NEW PropertyConverterString(), NEW PropertyFeelDefault()));

    Property* materialProp(NEW Property());
    materialProp->init<he::String>(HEFS::strMaterial, "");
    desc.m_Properties.setAt(materialProp->getName(), PropertyDesc(materialProp, "Material", "The material to use", 
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
