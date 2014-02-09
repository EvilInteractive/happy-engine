//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/12/27
#include "HappyPCH.h"
#include "MaterialInstance.h"

#include "Material.h"
#include "Drawable.h"
#include "Shader.h"
#include "ShaderUniform.h"
#include "DrawContext.h"
#include "ModelMesh.h"

namespace
{
    void applyShaderVar(he::gfx::Shader* const shader, const he::gfx::MaterialParameter& var)
    {
        he::gfx::IShaderUniform* uniform(shader->getUniform(var.getID()));
        HE_ASSERT(uniform != nullptr, "Could not find corresponding shader uniform when trying to set materialparam!");
        switch (var.getType())
        {
            case he::gfx::MaterialParameter::eType_Float: checked_cast<he::gfx::ShaderUniformFloat*>(uniform)->set(shader, var.getFloat()); break;
            case he::gfx::MaterialParameter::eType_Float2: checked_cast<he::gfx::ShaderUniformVec2*>(uniform)->set(shader, var.getFloat2()); break;
            case he::gfx::MaterialParameter::eType_Float3: checked_cast<he::gfx::ShaderUniformVec3*>(uniform)->set(shader, var.getFloat3()); break;
            case he::gfx::MaterialParameter::eType_Float4: checked_cast<he::gfx::ShaderUniformVec4*>(uniform)->set(shader, var.getFloat4()); break;
            case he::gfx::MaterialParameter::eType_Int: checked_cast<he::gfx::ShaderUniformInt*>(uniform)->set(shader, var.getInt()); break;
            case he::gfx::MaterialParameter::eType_Texture2D: checked_cast<he::gfx::ShaderUniformTexture2D*>(uniform)->set(shader, var.getTexture2D()); break;
            case he::gfx::MaterialParameter::eType_TextureCube: checked_cast<he::gfx::ShaderUniformTextureCube*>(uniform)->set(shader, var.getTextureCube()); break;
            case he::gfx::MaterialParameter::eType_Invalid: LOG(he::LogType_ProgrammerAssert, "Trying to set invalid material parameter!"); break;
            default: LOG(he::LogType_ProgrammerAssert, "Trying to set unknown material parameter type, %s", var.typeToString(var.getType())); break;
        }
    }
}

namespace he {
namespace gfx {

MaterialInstance::MaterialInstance(const Material* const material, const EShaderType type)
    : m_Material(material)
    , m_Type(type)
{
    m_Material->instantiate();
    m_Material->callbackOnceIfLoaded(this, [this](const ELoadResult result)
    {
        if (result == eLoadResult_Success)
        {
            init();
        }
        setLoaded(result);
    });
}

void MaterialInstance::init()
{
    // Copy everything from parent material
    m_Flags = m_Material->m_Flags;
    m_BlendEquation = m_Material->m_BlendEquation;
    m_SourceBlend = m_Material->m_SourceBlend;
    m_DestBlend = m_Material->m_DestBlend;
    
    m_Parameters.clear();
    
    Shader* const shader(m_Material->getShader(m_Type));
    const PrimitiveList<IShaderUniform*>& uniforms(shader->getUniforms());
    const size_t count(uniforms.size());
    for (size_t i(0); i < count; ++i)
    {
        IShaderUniform* const uniform(uniforms[i]);
        
        MaterialParameter param;
        const ShaderUniformID id(checked_numcast<uint32>(i));
        
        switch (uniform->getType())
        {
            case eShaderUniformType_Invalid: LOG(LogType_ProgrammerAssert, "Found invalid shader uniform? %s", uniform->getName().c_str()); break;
            case eShaderUniformType_Int:
                param.init(id, MaterialParameter::eType_Int);
                param.setInt32(checked_cast<ShaderUniformInt*>(uniform)->getValue());
                break;
            case eShaderUniformType_UInt:
                LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                break;
            case eShaderUniformType_Float:
                param.init(id, MaterialParameter::eType_Float);
                param.setFloat(checked_cast<ShaderUniformFloat*>(uniform)->getValue());
                break;
            case eShaderUniformType_Float2:
                param.init(id, MaterialParameter::eType_Float2);
                param.setFloat2(checked_cast<ShaderUniformVec2*>(uniform)->getValue());
                break;
            case eShaderUniformType_Float3:
                param.init(id, MaterialParameter::eType_Float3);
                param.setFloat3(checked_cast<ShaderUniformVec3*>(uniform)->getValue());
                break;
            case eShaderUniformType_Float4:
                param.init(id, MaterialParameter::eType_Float4);
                param.setFloat4(checked_cast<ShaderUniformVec4*>(uniform)->getValue());
                break;
            case eShaderUniformType_Mat44:
                LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                break;
            case eShaderUniformType_Mat44Array:
                LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
                break;
            case eShaderUniformType_Texture1D:
                LOG(LogType_ProgrammerAssert, "Found not implemented material parameter, ignoring... (%s)", uniform->getName().c_str()); break;
            case eShaderUniformType_Texture2D:
                param.init(id, MaterialParameter::eType_Texture2D);
                //param.setTexture2D(m_Material->getDefaultTexture2D(uniform->getName()));
                break;
            case eShaderUniformType_TextureCube:
                param.init(id, MaterialParameter::eType_TextureCube);
                //param.setTextureCube(m_Material->getDefaultTextureCube(uniform->getName()));
                break;
        }
        m_Parameters.add(param);
    }
}

MaterialInstance::~MaterialInstance()
{
    m_Material->cancelLoadCallback(this);
    m_Material->release();
}
    
void MaterialInstance::apply( const DrawContext& context ) const
{
    HE_IF_ASSERT(m_Type != eShaderType_Unknown, "You forgot to init this material instance! without the init we cannot draw this!")
    {
        applyShader(m_Type, context);
        applyMesh(m_Type, context);
    }
}

void MaterialInstance::applyShader( const EShaderType type, const DrawContext& context ) const
{
    if (checkFlag(eMaterialFlags_Blended))
    {
        GL::heBlendEquation(m_BlendEquation);
        GL::heBlendFunc(m_SourceBlend, m_DestBlend);
    }
    GL::heSetDepthRead(checkFlag(eMaterialFlags_DepthRead));
    GL::heSetDepthWrite(checkFlag(eMaterialFlags_DepthWrite));
    
    GL::heSetCullFace(checkFlag(eMaterialFlags_CullFrontFace));
    
    Shader* shader(m_Material->bindShader(type));

    m_Parameters.forEach(std::bind(::applyShaderVar, shader, std::placeholders::_1));
}

void MaterialInstance::applyMesh( const EShaderType type, const DrawContext& context ) const
{
    const ModelMesh* const mesh(context.m_CurrentDrawable->getModelMesh());
    const MaterialLayout& materialLayout(*context.m_CurrentDrawable->getMaterialLayout());
    const MaterialLayout::layout& elements(materialLayout.m_Layout[type]);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBOID());
    elements.forEach([type](const details::MaterialLayoutElement& e)
    {
        glVertexAttribPointer(e.m_ElementIndex, e.m_Components, type, GL_FALSE,
                              e.m_Stride, BUFFER_OFFSET(e.m_ByteOffset));
        glEnableVertexAttribArray(e.m_ElementIndex);
    });
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getVBOIndexID());
}

void MaterialInstance::calculateMaterialLayout( const BufferLayout& bufferLayout, MaterialLayout& outMaterialLayout ) const
{
    m_Material->calculateMaterialLayout(bufferLayout, outMaterialLayout);
}
    
} } //end namespace
