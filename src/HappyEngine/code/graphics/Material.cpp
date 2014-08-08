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
#include "PropertyConverter.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

Material::Material()
    : m_Flags(0)
    , m_BlendEquation(BlendEquation_Add)
    , m_SourceBlend(BlendFunc_One)
    , m_DestBlend(BlendFunc_Zero)
    , m_DepthFunc(DepthFunc_LessOrEqual)
{
    for (size_t i(0); i < eShaderType_MAX; ++i)
    {
        m_Shader[i] = nullptr;
    }
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
                        for (size_t i(0); i < eShaderType_MAX; ++i)
                        {
                            Shader* const shader(m_Shader[i]);
                            if (shader != nullptr)
                            {
                                const PrimitiveList<IShaderUniform*>& uniforms(shader->getUniforms());
                                const size_t count(uniforms.size());
                                for (size_t j(0); j < count; ++j)
                                {
                                    IShaderUniform* const uniform(uniforms[j]);

                                    MaterialParameter param;
                                    const ShaderUniformID id(checked_numcast<uint32>(j));

                                    size_t defaultIndex(SIZE_T_MAX);
                                    m_DefaultParams.find_if([uniform](const NameValuePair<he::String>& p) { return p.m_Name == uniform->getName(); }, defaultIndex);

                                    switch (uniform->getType())
                                    {
                                    case eShaderUniformType_Invalid: LOG(LogType_ProgrammerAssert, "Found invalid shader uniform? %s", uniform->getName().c_str()); break;
                                    case eShaderUniformType_Int:
                                        param.init(id, MaterialParameter::eType_Int);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            param.setInt32(he::ge::PropertyConverterInt::fromString(m_DefaultParams[defaultIndex].m_Value));
                                        }
                                        else
                                        {
                                            param.setInt32(checked_cast<ShaderUniformInt*>(uniform)->getValue());
                                        }
                                        break;
                                    case eShaderUniformType_UInt:
                                        LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                        break;
                                    case eShaderUniformType_Float:
                                        param.init(id, MaterialParameter::eType_Float);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            param.setFloat(he::ge::PropertyConverterFloat::fromString(m_DefaultParams[defaultIndex].m_Value));
                                        }
                                        else
                                        {
                                            param.setFloat(checked_cast<ShaderUniformFloat*>(uniform)->getValue());
                                        }
                                        break;
                                    case eShaderUniformType_Float2:
                                        param.init(id, MaterialParameter::eType_Float2);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            param.setFloat2(he::ge::PropertyConverterVec2::fromString(m_DefaultParams[defaultIndex].m_Value));
                                        }
                                        else
                                        {
                                            param.setFloat2(checked_cast<ShaderUniformVec2*>(uniform)->getValue());
                                        }
                                        break;
                                    case eShaderUniformType_Float3:
                                        param.init(id, MaterialParameter::eType_Float3);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            param.setFloat3(he::ge::PropertyConverterVec3::fromString(m_DefaultParams[defaultIndex].m_Value));
                                        }
                                        else
                                        {
                                            param.setFloat3(checked_cast<ShaderUniformVec3*>(uniform)->getValue());
                                        }
                                        break;
                                    case eShaderUniformType_Float4:
                                        param.init(id, MaterialParameter::eType_Float4);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            param.setFloat4(he::ge::PropertyConverterVec4::fromString(m_DefaultParams[defaultIndex].m_Value));
                                        }
                                        else
                                        {
                                            param.setFloat4(checked_cast<ShaderUniformVec4*>(uniform)->getValue());
                                        }
                                        break;
                                    case eShaderUniformType_Mat44:
                                        param.init(id, MaterialParameter::eType_Float44);
                                        param.setFloat44(checked_cast<ShaderUniformMat44*>(uniform)->getValue());
                                        break;
                                    case eShaderUniformType_Mat44Array:
                                        LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                        break;
                                    case eShaderUniformType_Texture1D:
                                        LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                    case eShaderUniformType_Texture2D:
                                        param.init(id, MaterialParameter::eType_Texture2D);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            const he::String& defaultValue(m_DefaultParams[defaultIndex].m_Value);
                                            if (defaultValue.empty() == false)
                                            {
                                                const Texture2D* tex(nullptr);
                                                if (isdigit(*defaultValue.c_str()))
                                                {
                                                    tex = CONTENT->asyncMakeTexture2D(he::Color(
                                                        he::ge::PropertyConverterVec4::fromString(defaultValue)));
                                                }
                                                else
                                                {
                                                    tex = CONTENT->asyncLoadTexture2D(defaultValue);
                                                }
                                                param.setTexture2D(tex);
                                                tex->release();
                                            }
                                        }
                                        break;
                                    case eShaderUniformType_TextureCube:
                                        param.init(id, MaterialParameter::eType_TextureCube);
                                        if (defaultIndex != SIZE_T_MAX)
                                        {
                                            const he::String& defaultValue(m_DefaultParams[defaultIndex].m_Value);
                                            if (defaultValue.empty() == false)
                                            {
                                                const TextureCube* tex(nullptr);
                                                if (isdigit(*defaultValue.c_str()))
                                                {
                                                    tex = CONTENT->asyncMakeTextureCube(he::Color(
                                                        he::ge::PropertyConverterVec4::fromString(defaultValue)));
                                                }
                                                else
                                                {
                                                    tex = CONTENT->asyncLoadTextureCube(defaultValue);
                                                }
                                                param.setTextureCube(tex);
                                                tex->release();
                                            }
                                        }
                                        break;
                                    }
                                    m_Parameters[i].add(param);
                                    m_ParameterNames[i].add(uniform->getName());
                                }
                            }
                        }
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
                HE_ASSERT(i + 1 < meshElementCount, "Invalid vertex buffer, could not find the data I was looking for!");
            }
        });
    }
}

void Material::setDepthFunc( const DepthFunc func )
{
    m_DepthFunc = func;
}

void Material::setDefaultParams( const he::ObjectList<NameValuePair<he::String>>& params )
{
    m_DefaultParams.clear();
    m_DefaultParams.append(params);
}

void Material::setDefaultParams( he::ObjectList<NameValuePair<he::String>>&& params )
{
    m_DefaultParams = std::forward<he::ObjectList<NameValuePair<he::String>>>(params);
}

} } //end namespace

