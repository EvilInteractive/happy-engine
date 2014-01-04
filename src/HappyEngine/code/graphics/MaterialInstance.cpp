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
    void applyShaderVar(he::gfx::Shader* const shader, he::gfx::MaterialParameter* const var)
    {
        he::gfx::IShaderUniform* uniform(shader->getUniform(var->getID()));
        HE_ASSERT(uniform != nullptr, "Could not find corresponding shader uniform when trying to set materialparam!");
        switch (var->getType())
        {
            case he::gfx::MaterialParameter::eType_Float: checked_cast<he::gfx::ShaderUniformFloat*>(uniform)->set(shader, var->getFloat()); break;
            case he::gfx::MaterialParameter::eType_Float2: checked_cast<he::gfx::ShaderUniformVec2*>(uniform)->set(shader, var->getFloat2()); break;
            case he::gfx::MaterialParameter::eType_Float3: checked_cast<he::gfx::ShaderUniformVec3*>(uniform)->set(shader, var->getFloat3()); break;
            case he::gfx::MaterialParameter::eType_Float4: checked_cast<he::gfx::ShaderUniformVec4*>(uniform)->set(shader, var->getFloat4()); break;
            case he::gfx::MaterialParameter::eType_Int: checked_cast<he::gfx::ShaderUniformInt*>(uniform)->set(shader, var->getInt()); break;
            case he::gfx::MaterialParameter::eType_Texture2D: checked_cast<he::gfx::ShaderUniformTexture2D*>(uniform)->set(shader, var->getTexture2D()); break;
            case he::gfx::MaterialParameter::eType_TextureCube: checked_cast<he::gfx::ShaderUniformTextureCube*>(uniform)->set(shader, var->getTextureCube()); break;
            case he::gfx::MaterialParameter::eType_Invalid: LOG(he::LogType_ProgrammerAssert, "Trying to set invalid material parameter!"); break;
            default: LOG(he::LogType_ProgrammerAssert, "Trying to set unknown material parameter type, %s", var->typeToString(var->getType())); break;
        }
    }
}

namespace he {
namespace gfx {

MaterialInstance::MaterialInstance(const Material* const material)
    : m_Material(material)
{
    m_Material->instantiate();
    m_Material->callbackOnceIfLoaded(this, [](const ELoadResult result)
    {
        if (result == eLoadResult_Success)
            init();
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
    
    m_ShaderCommonVars.clear();
    m_ShaderCommonVars.append(m_Material->m_ShaderCommonVars);
    for (size_t i(0); i < eShaderType_MAX; ++i)
    {
        m_ShaderSpecificVars[i].clear();
        m_ShaderSpecificVars[i].append(m_Material->m_ShaderSpecificVars[i]);
    }
}
    
void MaterialInstance::setLoaded(ELoadResult result)
{
    // do callbacks
    result;
}
    
MaterialInstance::~MaterialInstance()
{
    m_Material->cancelLoadCallback(this);
    m_Material->release();
}
    
void MaterialInstance::applyNormal( const DrawContext& context ) const
{
    applyShader(eShaderType_Normal, context);
    applyMesh(eShaderType_Normal, context);
}

void MaterialInstance::applySkinned( const DrawContext& context ) const
{
    applyShader(eShaderType_Skinned, context);
    applyMesh(eShaderType_Skinned, context);
}

void MaterialInstance::applyInstanced( const DrawContext& context ) const
{
    applyShader(eShaderType_Instanced, context);
    applyMesh(eShaderType_Instanced, context);
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
    
    m_ShaderCommonVars.forEach(std::bind(::applyShaderVar, shader, _1));
    m_ShaderSpecificVars[type].forEach(std::bind(::applyShaderVar, shader, _1));
}

void MaterialInstance::applyMesh( const EShaderType type, const DrawContext& context ) const
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

void MaterialInstance::calculateMaterialLayout( const BufferLayout& bufferLayout, MaterialLayout& outMaterialLayout ) const
{
    m_Material->calculateMaterialLayout(bufferLayout, outMaterialLayout);
}
    
} } //end namespace
