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

#include "Drawable.h"
#include "LightManager.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "Shader.h"
#include "ShaderVar.h"
#include "ICamera.h"
#include "Scene.h"
#include "ModelMesh.h"
#include "DrawContext.h"

namespace
{
    void applyShaderVar(he::gfx::Shader* const shader, he::gfx::ShaderVar* const var, 
        const he::gfx::Drawable* const drawable, const he::gfx::ICamera* const camera)
    {
        switch (var->getType())
        {      
        case he::gfx::ShaderVarType_User:
            {
                var->assignData(shader);
            } break;
        case he::gfx::ShaderVarType_WorldViewProjection: 
            {
                shader->setShaderVar(var->getId(), camera->getViewProjection() * drawable->getWorldMatrix()); 
            } break;
        case he::gfx::ShaderVarType_ViewProjection: 
            {
                shader->setShaderVar(var->getId(), camera->getViewProjection()); 
            } break;
        case he::gfx::ShaderVarType_World: 
            {
                shader->setShaderVar(var->getId(), camera->getView() * drawable->getWorldMatrix()); 
            } break;
        case he::gfx::ShaderVarType_WorldView: 
            {
                shader->setShaderVar(var->getId(), camera->getView() * drawable->getWorldMatrix()); 
            } break;
        case he::gfx::ShaderVarType_View: 
            {
                shader->setShaderVar(var->getId(), camera->getView()); 
            } break;
        case he::gfx::ShaderVarType_BoneTransforms: 
            {
                HE_NOT_IMPLEMENTED
                    //shader->setShaderVar(var->getId(), drawable->getModelMesh()->getBoneTransforms()); 
            } break;
        case he::gfx::ShaderVarType_WorldPosition: 
            {
                he::vec3 worldPos;
                drawable->getWorldMatrix().getTranslationComponent(worldPos);
                shader->setShaderVar(var->getId(), worldPos); 
            } break;

        case he::gfx::ShaderVarType_AmbientColor: 
            {
                shader->setShaderVar(var->getId(), he::vec4(drawable->getScene()->getLightManager()->getAmbientLight()->color, drawable->getScene()->getLightManager()->getAmbientLight()->multiplier)); 
            } break;
        case he::gfx::ShaderVarType_DirectionalColor: 
            {
                shader->setShaderVar(var->getId(), he::vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getColor(), drawable->getScene()->getLightManager()->getDirectionalLight()->getMultiplier())); 
            } break;
        case he::gfx::ShaderVarType_DirectionalDirection: 
            {
                shader->setShaderVar(var->getId(), (camera->getView() * he::vec4(drawable->getScene()->getLightManager()->getDirectionalLight()->getDirection(), 0)).xyz()); 
            } break;
        case he::gfx::ShaderVarType_NearFar: 
            {
                shader->setShaderVar(var->getId(), he::vec2(camera->getNearClip(), camera->getFarClip())); 
            } break;

        default: 
            {
                LOG(he::LogType_ProgrammerAssert, "unknown shaderVartype when preparing for draw");
            } break;
        }
    }
}

namespace he {
namespace gfx {

Material::Material()
    : m_Flags(0)
    , m_SourceBlend(BlendFunc_One)
    , m_DestBlend(BlendFunc_Zero)
    , m_BlendEquation(BlendEquation_Add)
{
}

Material::~Material()
{
    m_ShaderCommonVars.forEach([](ShaderVar* const var)
    {
        delete var;
    });
    for (size_t i(0); i < eShaderType_MAX; ++i)
    {
        m_ShaderSpecificVars[i].forEach([](ShaderVar* const var)
        {
            delete var;
        });
        if (m_Shader[i] != nullptr)
        {
            m_Shader[i]->release();
            m_Shader[i] = nullptr;
        }
    }
}

void Material::registerCommonVar( ShaderVar* const var )
{
    m_ShaderCommonVars.add(var);
}

void Material::registerSpecificVar( const EShaderType type, ShaderVar* const var )
{
    m_ShaderSpecificVars[type].add(var);
}

ShaderVar* Material::getVar( const he::FixedString& var )
{
    he::PrimitiveList<ShaderVar*>::const_iterator it(m_ShaderCommonVars.cbegin());
    for (; it != m_ShaderCommonVars.cend(); ++it)
    {
        if ((*it)->getName() == var)
        {          
            return *it;
        }
    }
    HE_ERROR("Unable to find var: %s", var.c_str());
    return nullptr;
}

void Material::setNormalShader( Shader* const shader )
{
    shader->instantiate();
    HE_ASSERT(m_Shader[eShaderType_Normal] == nullptr, "Normal shader already initialized!");
    m_Shader[eShaderType_Normal] = shader;
}

void Material::setSkinnedShader( Shader* const shader )
{
    shader->instantiate();
    HE_ASSERT(m_Shader[eShaderType_Skinned] == nullptr, "Skinned shader already initialized!");
    m_Shader[eShaderType_Skinned] = shader;
}

void Material::setInstancedShader( Shader* const shader, const BufferLayout& /*instancingLayout*/ )
{
    shader->instantiate();
    HE_ASSERT(m_Shader[eShaderType_Instanced] == nullptr, "Instanced shader already initialized!");
    m_Shader[eShaderType_Instanced] = shader;
    //m_InstanceLayout = instancingLayout;
}

void Material::applyNormal( const DrawContext& context ) const
{
    applyShader(eShaderType_Normal, context);
    applyMesh(eShaderType_Normal, context);
}

void Material::applySkinned( const DrawContext& context ) const
{
    applyShader(eShaderType_Skinned, context);
    applyMesh(eShaderType_Skinned, context);
}

void Material::applyInstanced( const DrawContext& context ) const
{
    applyShader(eShaderType_Instanced, context);
    applyMesh(eShaderType_Instanced, context);
}

void Material::applyShader( const EShaderType type, const DrawContext& context ) const
{
    if (checkFlag(eMaterialFlags_Blended))
    {
        GL::heBlendEquation(m_BlendEquation);
        GL::heBlendFunc(m_SourceBlend, m_DestBlend);
    }
    GL::heSetDepthRead(checkFlag(eMaterialFlags_DepthRead));
    GL::heSetDepthWrite(checkFlag(eMaterialFlags_DepthWrite));

    GL::heSetCullFace(checkFlag(eMaterialFlags_CullFrontFace));

    Shader* const shader(m_Shader[type]);
    shader->bind();
    m_ShaderCommonVars.forEach(boost::bind(::applyShaderVar, shader, _1, context.m_CurrentDrawable, context.m_Camera));
    m_ShaderSpecificVars[type].forEach(boost::bind(::applyShaderVar, shader, _1, context.m_CurrentDrawable, context.m_Camera));
}

void Material::applyMesh( const EShaderType type, const DrawContext& context ) const
{
    const ModelMesh* const mesh(context.m_CurrentDrawable->getModelMesh());
    const MaterialLayout& materialLayout(*context.m_CurrentDrawable->getMaterialLayout());
    const MaterialLayout::layout& elements(materialLayout.m_Layout[type]);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBOID());
    std::for_each(elements.cbegin(), elements.cend(), [&](const details::MaterialLayoutElement& e)
    {
        glVertexAttribPointer(e.m_ElementIndex, e.m_Components, type, GL_FALSE, 
            e.m_Stride, BUFFER_OFFSET(e.m_ByteOffset)); 
        glEnableVertexAttribArray(e.m_ElementIndex);
    });

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getVBOIndexID());
}

void Material::setIsBlended( bool isBlended, BlendEquation equation/* = BlendEquation_Add*/, 
                                             BlendFunc sourceBlend/*  = BlendFunc_One*/,
                                             BlendFunc destBlend/*    = BlendFunc_Zero*/ )
{
    if (isBlended)
    {
        raiseFlag(eMaterialFlags_Blended);
        m_BlendEquation = equation;
        m_SourceBlend = sourceBlend;
        m_DestBlend = destBlend;
    }
    else
    {
        clearFlag(eMaterialFlags_Blended);
    }
}

void Material::calculateMaterialLayout( const BufferLayout& bufferLayout, MaterialLayout& outMaterialLayout )
{
    const BufferLayout::layout& meshElements(bufferLayout.getElements());
    const size_t meshElementCount(meshElements.size());
    const size_t stride(bufferLayout.getSize());
    
    for (size_t t(0); t < eShaderType_MAX; ++t)
    {
        MaterialLayout::layout& elements(outMaterialLayout.m_Layout[t]);
        elements.clear();

        const ShaderLayout::layout& shaderElements(m_Shader[t]->getShaderLayout().getElements());

        std::for_each(shaderElements.cbegin(), shaderElements.cend(), [&](const ShaderLayoutElement& e)
        {
            for (size_t i(0); i < meshElementCount; ++i)
            {
                const BufferElement& meshElement(meshElements[i]);
                if (meshElement.getUsage() == e.getUsage())
                {
                    details::MaterialLayoutElement matEl;
                    GLint components(1);
                    GLenum type(0);
                    GL::getGLTypesFromBufferElement(meshElement, components, type);

                    matEl.m_ElementIndex = checked_numcast<uint16>(e.getElementIndex());
                    matEl.m_Components = checked_numcast<uint8>(components);
                    matEl.m_Type = checked_numcast<uint16>(type);
                    matEl.m_Stride = checked_numcast<uint16>(stride);
                    matEl.m_ByteOffset = checked_numcast<uint8>(meshElement.getByteOffset());
                    elements.add(matEl);

                    break;
                }
                LOG(LogType_ProgrammerAssert, "Invalid vertex buffer, could not find the data I was looking for!");
            }
        });
    }
}

} } //end namespace

