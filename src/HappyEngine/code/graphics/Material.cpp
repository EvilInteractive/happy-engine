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
//Created: 30/09/2011
#include "HappyPCH.h" 
#include "Material.h"

#include "Shader.h"
#include "MaterialInstance.h"
#include "ShaderUniform.h"

namespace he {
namespace gfx {

Material::Material()
    : m_Flags(0)
    , m_BlendEquation(BlendEquation_Add)
    , m_SourceBlend(BlendFunc_One)
    , m_DestBlend(BlendFunc_Zero)
    , m_DepthFunc(DepthFunc_LessOrEqual)
{
}

Material::~Material()
{
    for (size_t i(0); i < eShaderType_MAX; ++i)
    {
        if (m_Shader[i] != nullptr)
        {
            m_Shader[i]->cancelLoadCallback(this);
            m_Shader[i]->release();
            m_Shader[i] = nullptr;
        }
    }
}
    
MaterialInstance* Material::createMaterialInstance(const EShaderType type) const
{
    MaterialInstance* instance(NEW MaterialInstance(this, type));
    return instance;
}
    
void Material::init()
{
    HE_ASSERT(isLoaded() == false, "Material is already loaded and cannot be inited again!");
    for (size_t i(0); i < eShaderType_MAX; ++i)
    {
        Shader* const shader(m_Shader[i]);
        if (shader != nullptr)
        {
            shader->callbackOnceIfLoaded(this, [this, shader](const ELoadResult result)
            {
                if (result == eLoadResult_Success)
                {
                    bool loadCompleted(true);
                    for (size_t i(0); i < eShaderType_MAX; ++i)
                    {
                        if (m_Shader[i] != nullptr)
                        {
                            if (m_Shader[i]->isLoaded() == false)
                            {
                                loadCompleted = false;
                                break;
                            }
                        }
                    }
                    if (loadCompleted)
                    {
                        setLoaded(eLoadResult_Success);
                    }
                }
                else
                {
                    // Shader load failed, fail load material
                    for (size_t i(0); i < eShaderType_MAX; ++i)
                    {
                        Shader* const cancelShader(m_Shader[i]);
                        if (cancelShader != nullptr && cancelShader != shader)
                        {
                            if (cancelShader->isLoaded() == false)
                            {
                                cancelShader->cancelLoadCallback(this);
                            }
                        }
                    }
                    setLoaded(eLoadResult_Failed);
                }
            });
        }
    }
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

void Material::setInstancedShader( Shader* const shader, const VertexLayout& /*instancingLayout*/ )
{
    shader->instantiate();
    HE_ASSERT(m_Shader[eShaderType_Instanced] == nullptr, "Instanced shader already initialized!");
    m_Shader[eShaderType_Instanced] = shader;
    //m_InstanceLayout = instancingLayout;
}
 
Shader* Material::bindShader(const EShaderType type) const
{
    Shader* const shader(m_Shader[type]);
    HE_ASSERT(m_Shader[type] != nullptr, "Trying to apply material %s with type %d but I do not have this type!", getName().c_str(), type);
    shader->bind();
    return shader;
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

void Material::calculateMaterialLayout( const VertexLayout& bufferLayout, MaterialLayout& outMaterialLayout ) const
{
    const VertexLayout::layout& meshElements(bufferLayout.getElements());
    const size_t meshElementCount(meshElements.size());
    const size_t stride(bufferLayout.getSize());
    
    for (size_t t(0); t < eShaderType_MAX; ++t)
    {
        MaterialLayout::layout& elements(outMaterialLayout.m_Layout[t]);
        elements.clear();
        
        if (m_Shader[t] == nullptr) // If we have a shader of this type
            continue;

        const ShaderLayout::AttributeLayoutList& shaderElements(m_Shader[t]->getShaderLayout().getAttributes());

        std::for_each(shaderElements.cbegin(), shaderElements.cend(), [&](const ShaderLayoutAttribute& e)
        {
            for (size_t i(0); i < meshElementCount; ++i)
            {
                const VertexElement& meshElement(meshElements[i]);
                if (meshElement.getAttribute() == e.getType())
                {
                    details::MaterialLayoutElement matEl;
                    GLint components(1);
                    GLenum type(0);
                    GL::getGLTypesFromVertexElement(meshElement, components, type);

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

void Material::setDepthFunc( const DepthFunc func )
{
    m_DepthFunc = func;
}

} } //end namespace

