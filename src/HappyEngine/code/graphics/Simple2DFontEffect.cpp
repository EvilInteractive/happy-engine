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

namespace he {
namespace gfx {

Simple2DFontEffect::Simple2DFontEffect() : m_pShader(nullptr)
{
}


Simple2DFontEffect::~Simple2DFontEffect()
{
    delete m_pShader;
}

void Simple2DFontEffect::load()
{
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));
    layout.addElement(ShaderLayoutElement(1, "inTexCoord"));

    m_pShader = NEW Shader();
    std::vector<std::string> shaderOutputs;
    shaderOutputs.push_back("outColor");
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    bool compiled = m_pShader->initFromFile(folder + "2D/simple2DFontShader.vert", 
                                            folder + "2D/simple2DFontShader.frag", layout, shaderOutputs);
    HE_ASSERT(compiled, "");

    m_ShaderWVPPos = m_pShader->getShaderVarId("matWVP");
    m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
    m_ShaderDepthPos = m_pShader->getShaderVarId("depth");
    m_ShaderFontColorPos = m_pShader->getShaderVarId("fontColor");
    m_ShaderTCOffsetPos = m_pShader->getShaderVarId("texCoordOffset");
    m_ShaderTCScalePos = m_pShader->getShaderVarId("texCoordScale");

    m_pShader->bind();
    mat44 MatWVP = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_pShader->setShaderVar(m_ShaderWVPPos, MatWVP);
}

void Simple2DFontEffect::begin() const
{
    m_pShader->bind();
}

void Simple2DFontEffect::end() const
{
}

void Simple2DFontEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_pShader->setShaderVar(m_ShaderWVPPos, mat);
}

void Simple2DFontEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_pShader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

void Simple2DFontEffect::setDepth(float depth) const
{
    m_pShader->setShaderVar(m_ShaderDepthPos, depth);
}

void Simple2DFontEffect::setFontColor(const Color& col) const
{
    m_pShader->setShaderVar(m_ShaderFontColorPos, col.rgba());
}

void Simple2DFontEffect::setTCOffset(const vec2& offset) const
{
    m_pShader->setShaderVar(m_ShaderTCOffsetPos, offset);
}

void Simple2DFontEffect::setTCScale(const vec2& scale) const
{
    m_pShader->setShaderVar(m_ShaderTCScalePos, scale);
}

} } //end namespace