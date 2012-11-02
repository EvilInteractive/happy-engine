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
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "Material.h"
#include "IDrawable.h"

#include "LightManager.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

#include "Shader.h"
#include "ShaderVar.h"

#include "ICamera.h"
#include "Scene.h"

namespace he {
namespace gfx {

Material::Material(): m_UsedForInstancing(false), m_IsBlended(false), m_NoPost(false), m_IsBackground(false),
    m_DepthRead(true), m_DepthWrite(true), m_ShaderHandle(ObjectHandle::unassigned), m_CullFrontFace(false)
{
}

Material::~Material()
{
    std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [](ShaderVar* var)
    {
        delete var;
    });
    if (m_ShaderHandle != ObjectHandle::unassigned)
        ResourceFactory<Shader>::getInstance()->release(m_ShaderHandle);
}

void Material::registerVar(ShaderVar* var)
{
    HE_ASSERT(!m_UsedForInstancing || (var->getType() == ShaderVarType_View ||var->getType() == ShaderVarType_ViewProjection || var->getType() >= ShaderVarType_AmbientColor), "ShaderVarType not supported for instancing");
    m_ShaderVar.push_back(var);
}

void Material::setShader(const ObjectHandle& shaderHandle, const BufferLayout& compatibleVertexLayout, const BufferLayout& compatibleInstancingLayout)
{
    m_UsedForInstancing = compatibleInstancingLayout.getSize() > 0;
    if (m_ShaderHandle != ObjectHandle::unassigned)
        ResourceFactory<Shader>::getInstance()->release(m_ShaderHandle);
    ResourceFactory<Shader>::getInstance()->instantiate(shaderHandle);
    m_ShaderHandle = shaderHandle;
    HE_ASSERT(compatibleVertexLayout.getSize() > 0, "VertexLayout size == 0!");
    m_CompatibleVL = compatibleVertexLayout;
    m_CompatibleIL = compatibleInstancingLayout;
}
ShaderVar* Material::getVar( const std::string& var )
{
    std::vector<ShaderVar*>::const_iterator it(m_ShaderVar.cbegin());
    for (; it != m_ShaderVar.cend(); ++it)
    {
        if ((*it)->getName() == var)
        {          
            return *it;
        }
    }
    HE_ERROR("Unable to find var: %s", var.c_str());
    return nullptr;
}


void Material::apply( const SingleDrawable* drawable, const ICamera* camera ) const
{
    HE_IF_ASSERT(m_ShaderHandle != ObjectHandle::unassigned, "set shader first!")
    {
        if (m_IsBlended)
        {
            GL::heBlendEquation(m_BlendEquation);
            GL::heBlendFunc(m_SourceBlend, m_DestBlend);
        }
        GL::heSetDepthRead(m_DepthRead);
        GL::heSetDepthWrite(m_DepthWrite);

        GL::heSetCullFace(m_CullFrontFace);

        Shader* shader(ResourceFactory<Shader>::getInstance()->get(m_ShaderHandle));

        shader->bind();
        std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](ShaderVar* pVar)
        {
            if (pVar->getType() == ShaderVarType_User)
            {
                pVar->assignData(shader);
            }
            else
            {
                switch (pVar->getType())
                {      
                    case ShaderVarType_WorldViewProjection: 
                        shader->setShaderVar(pVar->getId(), camera->getViewProjection() * drawable->getWorldMatrix()); 
                        break;
                    case ShaderVarType_ViewProjection: 
                        shader->setShaderVar(pVar->getId(), camera->getViewProjection()); 
                        break;
                    case ShaderVarType_World: 
                        shader->setShaderVar(pVar->getId(), camera->getView() * drawable->getWorldMatrix()); 
                        break;
                    case ShaderVarType_WorldView: 
                        shader->setShaderVar(pVar->getId(), camera->getView() * drawable->getWorldMatrix()); 
                        break;   
                    case ShaderVarType_View: 
                        shader->setShaderVar(pVar->getId(), camera->getView()); 
                        break;             
                    case ShaderVarType_WorldPosition: 
                        {
                            vec3 worldPos;
                            drawable->getWorldMatrix().getTranslationComponent(worldPos);
                            shader->setShaderVar(pVar->getId(), worldPos); 
                            break;
                        }
                    
                    case ShaderVarType_AmbientColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getAmbientLight()->color, drawable->getScene()->getLightManager()->getAmbientLight()->multiplier)); 
                        break;
                    case ShaderVarType_DirectionalColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getColor(), drawable->getScene()->getLightManager()->getDirectionalLight()->getMultiplier())); 
                        break;
                    case ShaderVarType_DirectionalDirection: 
                        shader->setShaderVar(pVar->getId(), (camera->getView() * vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                        break;
                    case ShaderVarType_NearFar: 
                        shader->setShaderVar(pVar->getId(), vec2(camera->getNearClip(), camera->getFarClip())); 
                        break;

                    default: HE_ASSERT(false, "unkown shaderVartype for single"); break;
                }
            }
        });
    }
}
void Material::apply( const InstancedDrawable* drawable, const ICamera* camera ) const
{
    HE_IF_ASSERT(m_ShaderHandle != ObjectHandle::unassigned, "set shader first!")
    HE_IF_ASSERT(m_UsedForInstancing, "shader not capable for instancing!")
    {
        if (m_IsBlended)
        {
            GL::heBlendEquation(m_BlendEquation);
            GL::heBlendFunc(m_SourceBlend, m_DestBlend);
        }
        GL::heSetDepthRead(m_DepthRead);
        GL::heSetDepthWrite(m_DepthWrite);

        GL::heSetCullFace(m_CullFrontFace);

        Shader* shader(ResourceFactory<Shader>::getInstance()->get(m_ShaderHandle));
        shader->bind();
        std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](ShaderVar* pVar)
        {
            if (pVar->getType() == ShaderVarType_User)
            {
                pVar->assignData(shader);
            }
            else
            {
                switch (pVar->getType())
                {      
                    case ShaderVarType_ViewProjection: 
                        shader->setShaderVar(pVar->getId(), camera->getViewProjection()); 
                        break;
                    case ShaderVarType_View: 
                        shader->setShaderVar(pVar->getId(), camera->getView()); 
                        break;  

                    case ShaderVarType_AmbientColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getAmbientLight()->color, drawable->getScene()->getLightManager()->getAmbientLight()->multiplier)); 
                        break;
                    case ShaderVarType_DirectionalColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getColor(), drawable->getScene()->getLightManager()->getDirectionalLight()->getMultiplier())); 
                        break;
                    case ShaderVarType_DirectionalDirection: 
                        shader->setShaderVar(pVar->getId(), (camera->getView() * vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                        break;

                    case ShaderVarType_NearFar: 
                        shader->setShaderVar(pVar->getId(), vec2(camera->getNearClip(), camera->getFarClip())); 
                        break;

                    default: HE_ASSERT(false, "unkown shaderVartype for instancing"); break;
                }
            }
        });
    }
}
void Material::apply( const SkinnedDrawable* drawable, const ICamera* camera ) const
{
    HE_IF_ASSERT(m_ShaderHandle != ObjectHandle::unassigned, "set shader first!")
    {
        if (m_IsBlended)
        {
            GL::heBlendEquation(m_BlendEquation);
            GL::heBlendFunc(m_SourceBlend, m_DestBlend);
        }

        GL::heSetDepthRead(m_DepthRead);
        GL::heSetDepthWrite(m_DepthWrite);

        GL::heSetCullFace(m_CullFrontFace);

        Shader* shader(ResourceFactory<Shader>::getInstance()->get(m_ShaderHandle));
        shader->bind();
        std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](ShaderVar* pVar)
        {
            if (pVar->getType() == ShaderVarType_User)
            {
                pVar->assignData(shader);
            }
            else
            {
                switch (pVar->getType())
                {      
                    case ShaderVarType_WorldViewProjection: 
                        shader->setShaderVar(pVar->getId(), camera->getViewProjection() * drawable->getWorldMatrix()); 
                        break;
                    case ShaderVarType_ViewProjection: 
                        shader->setShaderVar(pVar->getId(), camera->getViewProjection()); 
                        break;
                    case ShaderVarType_View: 
                        shader->setShaderVar(pVar->getId(), camera->getView()); 
                        break;  
                    case ShaderVarType_World: 
                        shader->setShaderVar(pVar->getId(), camera->getView() * drawable->getWorldMatrix()); 
                        break;
                    case ShaderVarType_WorldView: 
                        shader->setShaderVar(pVar->getId(), camera->getView() * drawable->getWorldMatrix()); 
                        break;
                    case ShaderVarType_BoneTransforms: 
                        shader->setShaderVar(pVar->getId(), drawable->getBoneTransforms()); 
                        break;

                    case ShaderVarType_WorldPosition: 
                        {
                            vec3 worldPos;
                            drawable->getWorldMatrix().getTranslationComponent(worldPos);
                            shader->setShaderVar(pVar->getId(), worldPos); 
                            break;
                        }

                    case ShaderVarType_AmbientColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getAmbientLight()->color, drawable->getScene()->getLightManager()->getAmbientLight()->multiplier)); 
                        break;
                    case ShaderVarType_DirectionalColor: 
                        shader->setShaderVar(pVar->getId(), vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getColor(), drawable->getScene()->getLightManager()->getDirectionalLight()->getMultiplier())); 
                        break;
                    case ShaderVarType_DirectionalDirection: 
                        shader->setShaderVar(pVar->getId(), (camera->getView() * vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                        break;

                    case ShaderVarType_NearFar: 
                        shader->setShaderVar(pVar->getId(), vec2(camera->getNearClip(), camera->getFarClip())); 
                        break;

                    default: HE_ASSERT(false, "unkown shaderVartype for skinning"); break;
                }
            }
        });
    }
}


const BufferLayout& Material::getCompatibleVertexLayout() const
{
    return m_CompatibleVL;
}
const BufferLayout& Material::getCompatibleInstancingLayout() const
{
    return m_CompatibleIL;
}



void Material::setIsBlended( bool isBlended, BlendEquation equation/* = BlendEquation_Add*/, 
                                             BlendFunc sourceBlend/*  = BlendFunc_One*/,
                                             BlendFunc destBlend/*    = BlendFunc_Zero*/ )
{
    m_IsBlended = isBlended;
    m_BlendEquation = equation;
    m_SourceBlend = sourceBlend;
    m_DestBlend = destBlend;
}

bool Material::isBlended() const
{
    return m_IsBlended;
}
BlendEquation Material::getBlendEquation() const
{
    return m_BlendEquation;
}
BlendFunc Material::getSourceBlend() const
{
    return m_SourceBlend;
}
BlendFunc Material::getDestBlend() const
{
    return m_DestBlend;
}

bool Material::isUsedForInstancing() const
{
    return m_UsedForInstancing;
}
void Material::setNoPost( bool noPost )
{
    m_NoPost = noPost;
}

bool Material::noPost() const
{
    return m_NoPost;
}

void Material::setIsBackground( bool isBackground )
{
    m_IsBackground = isBackground;
    if (m_IsBackground)
        m_NoPost = true;
}

bool Material::isBackground() const
{
    return m_IsBackground;
}

void Material::setDepthWriteEnabled( bool enable )
{
    m_DepthWrite = enable;
}

void Material::setDepthReadEnabled( bool enable )
{
    m_DepthRead = enable;
}

void Material::setCullFrontFace( bool enable )
{
    m_CullFrontFace = enable;
}




} } //end namespace
