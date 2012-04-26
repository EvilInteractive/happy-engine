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

#include "BillboardEffect.h"
#include "HappyNew.h"
#include "HeAssert.h"
#include "ContentManager.h"

namespace he {
namespace gfx {

BillboardEffect::BillboardEffect() : m_pShader(nullptr)
{
}


BillboardEffect::~BillboardEffect()
{
    delete m_pShader;
}

void BillboardEffect::load()
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
    bool compiled = m_pShader->initFromFile(folder + "2D/billboardShader.vert", 
                                            folder + "2D/billboardShader.frag", layout, shaderOutputs);
    HE_ASSERT(compiled, "");

    m_ShaderWVPPos = m_pShader->getShaderVarId("matWVP");
    m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
    m_ShaderTCScalePos = m_pShader->getShaderVarId("texCoordScale");

    m_pShader->bind();
    mat44 MatWVP = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    setWorldViewProjection(MatWVP);
    setTCScale(vec2(1.0f,1.0f));
}

void BillboardEffect::begin() const
{
    m_pShader->bind();
}

void BillboardEffect::end() const
{
}

void BillboardEffect::setWorldViewProjection(const he::mat44& mat) const
{
    m_pShader->setShaderVar(m_ShaderWVPPos, mat);
}

void BillboardEffect::setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const
{
    m_pShader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

void BillboardEffect::setTCScale(const vec2& scale) const
{
    m_pShader->setShaderVar(m_ShaderTCScalePos, scale);
}

} } //end namespace