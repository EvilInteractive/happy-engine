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

Material::Material(): m_UsedForInstancing(false), m_IsTranslucent(false), m_NoPost(false)
{
}


Material::~Material()
{
}

void Material::addVar(const ShaderVar::pointer& var)
{
    ASSERT(!m_UsedForInstancing || (var->getType() == ShaderVarType_View ||var->getType() == ShaderVarType_ViewProjection || var->getType() >= ShaderVarType_AmbientColor), "ShaderVarType not supported for instancing");
    m_ShaderVar.push_back(var);
}
void Material::setShader(const Shader::pointer& pShader, const VertexLayout& compatibleVL, bool usedForInstancing)
{
    m_UsedForInstancing = usedForInstancing;
    m_pShader = pShader;
    m_CompatibleVL = compatibleVL;
}

void Material::apply( const ISingleDrawable* pDrawable, const ICamera* pCamera ) const
{
    ASSERT(m_pShader != nullptr, "set shader first!");
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

                default: ASSERT(false, "unkown shaderVartype for single"); break;
            }
        }
    });
}
void Material::apply( const IInstancedDrawable* /*pDrawable*/, const ICamera* pCamera ) const
{
    ASSERT(m_pShader != nullptr, "set shader first!");
    ASSERT(m_UsedForInstancing, "shader not capable for instancing!");
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

                default: ASSERT(false, "unkown shaderVartype for instancing"); break;
            }
        }
    });
}
void Material::apply( const ISkinnedDrawable* pDrawable, const ICamera* pCamera ) const
{
    ASSERT(m_pShader != nullptr, "set shader first!");
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

                default: ASSERT(false, "unkown shaderVartype for skinning"); break;
            }
        }
    });
}


const VertexLayout& Material::getCompatibleVertexLayout() const
{
    return m_CompatibleVL;
}

bool Material::isTranslucent() const
{
    return m_IsTranslucent;
}

bool Material::isUsedForInstancing() const
{
    return m_UsedForInstancing;
}

void Material::setIsTranslucent( bool isTranslucent )
{
    m_IsTranslucent = isTranslucent;
}

void Material::setNoPost( bool noPost )
{
    m_NoPost = noPost;
}

bool Material::noPost() const
{
    return m_NoPost;
}

} } //end namespace