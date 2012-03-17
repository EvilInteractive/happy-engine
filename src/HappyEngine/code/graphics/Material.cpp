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
#include "HappyNew.h"
#include "IDrawable.h"

#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "LightManager.h"
#include "Light.h"

#include "ICamera.h"

namespace he {
namespace gfx {

Material::Material(): m_UsedForInstancing(false), m_IsBlended(false), m_NoPost(false), m_IsBackground(false),
    m_DepthRead(true), m_DepthWrite(true)
{
}


Material::~Material()
{
}

void Material::addVar(const ShaderVar::pointer& var)
{
    HE_ASSERT(!m_UsedForInstancing || (var->getType() == ShaderVarType_View ||var->getType() == ShaderVarType_ViewProjection || var->getType() >= ShaderVarType_AmbientColor), "ShaderVarType not supported for instancing");
    m_ShaderVar.push_back(var);
}

void Material::setShader(const Shader::pointer& pShader, const BufferLayout& compatibleVertexLayout, const BufferLayout& compatibleInstancingLayout)
{
    m_UsedForInstancing = compatibleInstancingLayout.getSize() > 0;
    m_pShader = pShader;
    HE_ASSERT(compatibleVertexLayout.getSize() > 0, "VertexLayout size == 0!");
    m_CompatibleVL = compatibleVertexLayout;
    m_CompatibleIL = compatibleInstancingLayout;
}
const ShaderVar::pointer& Material::getVar( const std::string& var )
{
    std::vector<ShaderVar::pointer>::const_iterator it(m_ShaderVar.cbegin());
    for (; it != m_ShaderVar.cend(); ++it)
    {
        if ((*it)->getName() == var)
        {          
            return *it;
        }
    }
    HE_ERROR("Unable to find var: " + var);
    return *it;
}


void Material::apply( const ISingleDrawable* pDrawable, const ICamera* pCamera ) const
{
    HE_ASSERT(m_pShader != nullptr, "set shader first!");

    if (m_IsBlended)
    {
        GL::heBlendEquation(m_BlendEquation);
        GL::heBlendFunc(m_SourceBlend, m_DestBlend);
    }
    GL::heSetDepthRead(m_DepthRead);
    GL::heSetDepthWrite(m_DepthWrite);

    m_pShader->bind();
    std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](const ShaderVar::pointer& pVar)
    {
        if (pVar->getType() == ShaderVarType_User)
        {
            pVar->assignData(m_pShader);
        }
        else
        {
            switch (pVar->getType())
            {      
                case ShaderVarType_WorldViewProjection: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection() * pDrawable->getWorldMatrix()); 
                    break;
                case ShaderVarType_ViewProjection: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection()); 
                    break;
                case ShaderVarType_World: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); 
                    break;
                case ShaderVarType_WorldView: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); 
                    break;   
                case ShaderVarType_View: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView()); 
                    break;             
                case ShaderVarType_WorldPosition: 
                    m_pShader->setShaderVar(pVar->getId(), pDrawable->getWorldMatrix().getTranslation()); 
                    break;
                    
                case ShaderVarType_AmbientColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getAmbientLight()->color, GRAPHICS->getLightManager()->getAmbientLight()->multiplier)); 
                    break;
                case ShaderVarType_DirectionalColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getColor(), GRAPHICS->getLightManager()->getDirectionalLight()->getMultiplier())); 
                    break;
                case ShaderVarType_DirectionalDirection: 
                    m_pShader->setShaderVar(pVar->getId(), (pCamera->getView() * vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                    break;

                case ShaderVarType_ShadowCascadeMatrix0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(0)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(1)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(2)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(3)); 
                    break;

                case ShaderVarType_ShadowCascade0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(0)); 
                    break;
                case ShaderVarType_ShadowCascade1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(1)); 
                    break;
                case ShaderVarType_ShadowCascade2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(2)); 
                    break;
                case ShaderVarType_ShadowCascade3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(3)); 
                    break;

                default: HE_ASSERT(false, "unkown shaderVartype for single"); break;
            }
        }
    });
}
void Material::apply( const IInstancedDrawable* /*pDrawable*/, const ICamera* pCamera ) const
{
    HE_ASSERT(m_pShader != nullptr, "set shader first!");
    HE_ASSERT(m_UsedForInstancing, "shader not capable for instancing!");

    if (m_IsBlended)
    {
        GL::heBlendEquation(m_BlendEquation);
        GL::heBlendFunc(m_SourceBlend, m_DestBlend);
    }
    GL::heSetDepthRead(m_DepthRead);
    GL::heSetDepthWrite(m_DepthWrite);

    m_pShader->bind();
    std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](const ShaderVar::pointer& pVar)
    {
        if (pVar->getType() == ShaderVarType_User)
        {
            pVar->assignData(m_pShader);
        }
        else
        {
            switch (pVar->getType())
            {      
                case ShaderVarType_ViewProjection: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection()); 
                    break;
                case ShaderVarType_View: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView()); 
                    break;  

                case ShaderVarType_AmbientColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getAmbientLight()->color, GRAPHICS->getLightManager()->getAmbientLight()->multiplier)); 
                    break;
                case ShaderVarType_DirectionalColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getColor(), GRAPHICS->getLightManager()->getDirectionalLight()->getMultiplier())); 
                    break;
                case ShaderVarType_DirectionalDirection: 
                    m_pShader->setShaderVar(pVar->getId(), (pCamera->getView() * vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                    break;

                case ShaderVarType_ShadowCascadeMatrix0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(0)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(1)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(2)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(3)); 
                    break;

                case ShaderVarType_ShadowCascade0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(0)); 
                    break;
                case ShaderVarType_ShadowCascade1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(1)); 
                    break;
                case ShaderVarType_ShadowCascade2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(2)); 
                    break;
                case ShaderVarType_ShadowCascade3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(3)); 
                    break;

                default: HE_ASSERT(false, "unkown shaderVartype for instancing"); break;
            }
        }
    });
}
void Material::apply( const ISkinnedDrawable* pDrawable, const ICamera* pCamera ) const
{
    HE_ASSERT(m_pShader != nullptr, "set shader first!");

    if (m_IsBlended)
    {
        GL::heBlendEquation(m_BlendEquation);
        GL::heBlendFunc(m_SourceBlend, m_DestBlend);
    }

    GL::heSetDepthRead(m_DepthRead);
    GL::heSetDepthWrite(m_DepthWrite);

    m_pShader->bind();
    std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](const ShaderVar::pointer& pVar)
    {
        if (pVar->getType() == ShaderVarType_User)
        {
            pVar->assignData(m_pShader);
        }
        else
        {
            switch (pVar->getType())
            {      
                case ShaderVarType_WorldViewProjection: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection() * pDrawable->getWorldMatrix()); 
                    break;
                case ShaderVarType_ViewProjection: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection()); 
                    break;
                case ShaderVarType_View: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView()); 
                    break;  
                case ShaderVarType_World: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); 
                    break;
                case ShaderVarType_WorldView: 
                    m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); 
                    break;
                case ShaderVarType_BoneTransforms: 
                    m_pShader->setShaderVar(pVar->getId(), pDrawable->getBoneTransforms()); 
                    break;

                case ShaderVarType_WorldPosition: m_pShader->setShaderVar(pVar->getId(), pDrawable->getWorldMatrix().getTranslation()); 
                    break;

                case ShaderVarType_AmbientColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getAmbientLight()->color, GRAPHICS->getLightManager()->getAmbientLight()->multiplier)); 
                    break;
                case ShaderVarType_DirectionalColor: 
                    m_pShader->setShaderVar(pVar->getId(), vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getColor(), GRAPHICS->getLightManager()->getDirectionalLight()->getMultiplier())); 
                    break;
                case ShaderVarType_DirectionalDirection: 
                    m_pShader->setShaderVar(pVar->getId(), (pCamera->getView() * vec4(GRAPHICS->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
                    break;

                case ShaderVarType_ShadowCascadeMatrix0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(0)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(1)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(2)); 
                    break;
                case ShaderVarType_ShadowCascadeMatrix3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMatrix(3)); 
                    break;

                case ShaderVarType_ShadowCascade0: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(0)); 
                    break;
                case ShaderVarType_ShadowCascade1: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(1)); 
                    break;
                case ShaderVarType_ShadowCascade2: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(2)); 
                    break;
                case ShaderVarType_ShadowCascade3: 
                    m_pShader->setShaderVar(pVar->getId(), GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(3)); 
                    break;

                default: HE_ASSERT(false, "unkown shaderVartype for skinning"); break;
            }
        }
    });
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




} } //end namespace