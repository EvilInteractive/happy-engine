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
    for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
    {
        for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
        {
            m_Shader[pass][rtype] = nullptr;
        }
    }
}

Material::~Material()
{
    for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
    {
        for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
        {
            Shader* const shader(m_Shader[pass][rtype]);
            if (shader != nullptr)
            {
                shader->cancelLoadCallback(this);
                shader->release();
                m_Shader[pass][rtype] = nullptr;
            }
        }
    }
}
    
MaterialInstance* Material::createMaterialInstance(const EShaderRenderType type) const
{
    MaterialInstance* instance(NEW MaterialInstance(this, type));
    return instance;
}
    
void Material::init()
{
    HE_ASSERT(isLoaded() == false, "Material is already loaded and cannot be inited again!");

    for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
    {
        for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
        {
            Shader* const shader(m_Shader[pass][rtype]);
            if (shader != nullptr)
            {
                shader->callbackOnceIfLoaded(this, [this, shader](const ELoadResult result)
                {
                    if (result == eLoadResult_Success)
                    {
                        bool loadCompleted(true);
                        for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
                        {
                            for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
                            {
                                Shader* const loadedShader(m_Shader[pass][rtype]);
                                if (loadedShader != nullptr)
                                {
                                    if (loadedShader->isLoaded() == false)
                                    {
                                        loadCompleted = false;
                                        break;
                                    }
                                }
                            }
                        }
                        if (loadCompleted)
                        {
                            for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
                            {
                                for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
                                {
                                    Shader* const shader(m_Shader[pass][rtype]);
                                    if (shader != nullptr)
                                    {
                                        const PrimitiveList<IShaderUniform*>& uniforms(shader->getUniforms());
                                        const size_t count(uniforms.size());
                                        for (size_t j(0); j < count; ++j)
                                        {
                                            IShaderUniform* const uniform(uniforms[j]);
                                            MaterialParameter* param;
                                            size_t index(0);
                                            if (m_ParameterNames.find(uniform->getName(), index))
                                            {
                                                param = &m_Parameters[index];
                                                HE_ASSERT(param->getType() == ShaderUniformTypeToMaterialType(uniform->getType()), "Material parameter name clash with variable of different type! (%s)", uniform->getName().c_str());
                                            }
                                            else
                                            {
                                                MaterialParameter tempParam;

                                                size_t defaultIndex(SIZE_T_MAX);
                                                m_DefaultParams.find_if([uniform](const NameValuePair<he::String>& p) { return p.m_Name == uniform->getName(); }, defaultIndex);

                                                switch (uniform->getType())
                                                {
                                                case eShaderUniformType_Invalid: LOG(LogType_ProgrammerAssert, "Found invalid shader uniform? %s", uniform->getName().c_str()); break;
                                                case eShaderUniformType_Int:
                                                    tempParam.init(MaterialParameter::eType_Int);
                                                    if (defaultIndex != SIZE_T_MAX)
                                                    {
                                                        tempParam.setInt32(he::ge::PropertyConverterInt::fromString(m_DefaultParams[defaultIndex].m_Value));
                                                    }
                                                    else
                                                    {
                                                        tempParam.setInt32(checked_cast<ShaderUniformInt*>(uniform)->getValue());
                                                    }
                                                    break;
                                                case eShaderUniformType_UInt:
                                                    LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                                    break;
                                                case eShaderUniformType_Float:
                                                    tempParam.init(MaterialParameter::eType_Float);
                                                    if (defaultIndex != SIZE_T_MAX)
                                                    {
                                                        tempParam.setFloat(he::ge::PropertyConverterFloat::fromString(m_DefaultParams[defaultIndex].m_Value));
                                                    }
                                                    else
                                                    {
                                                        tempParam.setFloat(checked_cast<ShaderUniformFloat*>(uniform)->getValue());
                                                    }
                                                    break;
                                                case eShaderUniformType_Float2:
                                                    tempParam.init(MaterialParameter::eType_Float2);
                                                    if (defaultIndex != SIZE_T_MAX)
                                                    {
                                                        tempParam.setFloat2(he::ge::PropertyConverterVec2::fromString(m_DefaultParams[defaultIndex].m_Value));
                                                    }
                                                    else
                                                    {
                                                        tempParam.setFloat2(checked_cast<ShaderUniformVec2*>(uniform)->getValue());
                                                    }
                                                    break;
                                                case eShaderUniformType_Float3:
                                                    tempParam.init(MaterialParameter::eType_Float3);
                                                    if (defaultIndex != SIZE_T_MAX)
                                                    {
                                                        tempParam.setFloat3(he::ge::PropertyConverterVec3::fromString(m_DefaultParams[defaultIndex].m_Value));
                                                    }
                                                    else
                                                    {
                                                        tempParam.setFloat3(checked_cast<ShaderUniformVec3*>(uniform)->getValue());
                                                    }
                                                    break;
                                                case eShaderUniformType_Float4:
                                                    tempParam.init(MaterialParameter::eType_Float4);
                                                    if (defaultIndex != SIZE_T_MAX)
                                                    {
                                                        tempParam.setFloat4(he::ge::PropertyConverterVec4::fromString(m_DefaultParams[defaultIndex].m_Value));
                                                    }
                                                    else
                                                    {
                                                        tempParam.setFloat4(checked_cast<ShaderUniformVec4*>(uniform)->getValue());
                                                    }
                                                    break;
                                                case eShaderUniformType_Mat44:
                                                    tempParam.init(MaterialParameter::eType_Float44);
                                                    tempParam.setFloat44(checked_cast<ShaderUniformMat44*>(uniform)->getValue());
                                                    break;
                                                case eShaderUniformType_Mat44Array:
                                                    LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                                    break;
                                                case eShaderUniformType_Texture1D:
                                                    LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                                                case eShaderUniformType_Texture2D:
                                                    tempParam.init(MaterialParameter::eType_Texture2D);
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
                                                            tempParam.setTexture2D(tex);
                                                            tex->release();
                                                        }
                                                    }
                                                    break;
                                                case eShaderUniformType_TextureCube:
                                                    tempParam.init(MaterialParameter::eType_TextureCube);
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
                                                            tempParam.setTextureCube(tex);
                                                            tex->release();
                                                        }
                                                    }
                                                    break;
                                                }

                                                m_Parameters.add(tempParam);
                                                m_ParameterNames.add(uniform->getName());
                                                param = &m_Parameters.back();
                                            }
                                            const ShaderUniformID id(checked_numcast<uint32>(j));
                                            param->setShaderUniformID(id, checked_numcast<EShaderPassType>(pass), checked_numcast<EShaderRenderType>(rtype));
                                        }
                                    }
                                }
                            }
                            setLoaded(eLoadResult_Success);
                        }
                    }
                    else
                    {
                        // Shader load failed, fail load material
                        for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
                        {
                            for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
                            {
                                Shader* const cancelShader(m_Shader[pass][rtype]);
                                if (cancelShader != nullptr && cancelShader != shader)
                                {
                                    if (cancelShader->isLoaded() == false)
                                    {
                                        cancelShader->cancelLoadCallback(this);
                                    }
                                }
                            }
                        }
                        setLoaded(eLoadResult_Failed);
                    }
                });
            }
        }
    }
}

void Material::setShader( const EShaderPassType pass, const EShaderRenderType renderType, Shader* const shader )
{
    shader->instantiate();
    HE_ASSERT(m_Shader[pass][renderType] == nullptr, "Shader[%d][%d] already initialized!", pass, renderType);
    m_Shader[pass][renderType] = shader;
}

Shader* Material::bindShader(const EShaderPassType pass, const EShaderRenderType renderType) const
{
    Shader* const shader(m_Shader[pass][renderType]);
    HE_ASSERT(shader != nullptr, "Trying to apply material %s with type [%d][%d] but I do not have this type!", getName().c_str(), pass, renderType);
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

    for (size_t pass(0); pass < eShaderPassType_MAX; ++pass)
    {
        for (size_t rtype(0); rtype < eShaderRenderType_MAX; ++rtype)
        {
            MaterialLayout::layout& elements(outMaterialLayout.m_Layout[pass][rtype]);
            elements.clear();
        
            if (m_Shader[pass][rtype] == nullptr) // If we have a shader of this type
                continue;

            const ShaderLayout::AttributeLayoutList& shaderElements(m_Shader[pass][rtype]->getShaderLayout().getAttributes());

            shaderElements.forEach([&](const ShaderLayoutAttribute& e)
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

