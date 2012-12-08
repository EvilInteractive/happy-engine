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
//Author:  Sebastiaan Sprengers
//Created: 07/05/2012

#include "HappyPCH.h" 

#include "Simple2DFontEffect.h"
#include "ContentManager.h"

#include "Shader.h"

namespace he {
namespace gfx {

Simple2DFontEffect::Simple2DFontEffect() : m_Shader(nullptr)
{
}


Simple2DFontEffect::~Simple2DFontEffect()
{
    if (m_Shader != nullptr)
        m_Shader->release();
}

void Simple2DFontEffect::load()
{
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));
    layout.addElement(ShaderLayoutElement(1, "inTexCoord"));
    layout.addElement(ShaderLayoutElement(2, "inColor"));

    m_Shader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    he::ObjectList<std::string> shaderOutputs;
    shaderOutputs.add("outColor");
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    const bool compiled = m_Shader->initFromFile(folder + "2D/simple2DFontShader.vert", 
                                                 folder + "2D/simple2DFontShader.frag", layout, shaderOutputs);
    HE_ASSERT(compiled, ""); compiled;

    m_ShaderWVPPos = m_Shader->getShaderVarId("matWVP");
    m_ShaderDiffTexPos = m_Shader->getShaderSamplerId("diffuseMap");
}

void Simple2DFontEffect::begin() const
{
    m_Shader->bind();
}

void Simple2DFontEffect::end() const
{
}

void Simple2DFontEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_Shader->setShaderVar(m_ShaderWVPPos, mat);
}

void Simple2DFontEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_Shader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

} } //end namespace