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
//Created: 26/08/2011
#include "HappyPCH.h" 

#include "Simple2DTextureEffect.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

Simple2DTextureEffect::Simple2DTextureEffect() : m_pShader(nullptr)
{
}


Simple2DTextureEffect::~Simple2DTextureEffect()
{
    delete m_pShader;
}

void Simple2DTextureEffect::load()
{
    using namespace he;
    using namespace gfx;

    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));
    layout.addElement(ShaderLayoutElement(1, "inTexCoord"));

    m_pShader = NEW Shader();
    std::vector<std::string> shaderOutputs;
    shaderOutputs.push_back("outColor");
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    bool compiled = m_pShader->initFromFile(folder + "2D/simple2DTextureShader.vert", 
                                            folder + "2D/simple2DTextureShader.frag", layout, shaderOutputs);
    HE_ASSERT(compiled, "");

    m_ShaderWVPPos = m_pShader->getShaderVarId("matWVP");
    m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
    m_ShaderAlphaPos = m_pShader->getShaderVarId("inAlpha");
    m_ShaderTCOffsetPos = m_pShader->getShaderVarId("texCoordOffset");
    m_ShaderTCScalePos = m_pShader->getShaderVarId("texCoordScale");
    m_ShaderDepthPos = m_pShader->getShaderVarId("depth");

    m_pShader->bind();
    mat44 MatWVP = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_pShader->setShaderVar(m_ShaderWVPPos, MatWVP);
}

void Simple2DTextureEffect::begin() const
{
    m_pShader->bind();
}

void Simple2DTextureEffect::end() const
{
}

void Simple2DTextureEffect::setWorldMatrix(const he::mat44& mat) const
{
    m_pShader->setShaderVar(m_ShaderWVPPos, mat);
}

void Simple2DTextureEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_pShader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

void Simple2DTextureEffect::setAlpha(const float alpha) const
{
    m_pShader->setShaderVar(m_ShaderAlphaPos, alpha);
}

void Simple2DTextureEffect::setTCOffset(const vec2& offset) const
{
    m_pShader->setShaderVar(m_ShaderTCOffsetPos, offset);
}

void Simple2DTextureEffect::setTCScale(const vec2& scale) const
{
    m_pShader->setShaderVar(m_ShaderTCScalePos, scale);
}

void Simple2DTextureEffect::setDepth(float depth) const
{
    m_pShader->setShaderVar(m_ShaderDepthPos, depth);
}

} } //end namespace