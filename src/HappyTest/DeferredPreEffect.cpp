//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
#include "DeferredPreEffect.h"
#include "VertexPNT.h"
#include "VertexLayout.h"
#include "HappyNew.h"

namespace happytest {

DeferredPreEffect::DeferredPreEffect(): m_pShader(nullptr)
{
}


DeferredPreEffect::~DeferredPreEffect()
{
    delete m_pShader;
}
happyengine::graphics::VertexLayout DeferredPreEffect::getVertexLayout()
{
    using namespace happyengine;
    using namespace graphics;
    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, "inTexCoord"));
    layout.addElement(VertexElement(2, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), 20, "inNormal"));
    layout.addElement(VertexElement(3, VertexElement::Type_Vector3, VertexElement::Usage_Tangent, sizeof(math::Vector3), 32, "inTangent"));

    return layout;
}
void DeferredPreEffect::load()
{
    using namespace happyengine;
    using namespace graphics;

    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, sizeof(math::Vector3), 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, sizeof(math::Vector2), 12, "inTexCoord"));
    layout.addElement(VertexElement(2, VertexElement::Type_Vector3, VertexElement::Usage_Normal, sizeof(math::Vector3), 20, "inNormal"));
    layout.addElement(VertexElement(3, VertexElement::Type_Vector3, VertexElement::Usage_Tangent, sizeof(math::Vector3), 32, "inTangent"));

    m_pShader = NEW Shader();
    std::vector<std::string> shaderOutputs;
    shaderOutputs.push_back("outColorIll");
    shaderOutputs.push_back("outPosSpec");
    shaderOutputs.push_back("outNormalGloss");
    ASSERT(m_pShader->init("../data/shaders/deferredPreShader.vert", "../data/shaders/deferredPreShader.frag", layout, shaderOutputs) == true);

    m_ShaderWVPpos = m_pShader->getShaderVarId("matWVP");
    m_ShaderWorldPos = m_pShader->getShaderVarId("matWorld");
    m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
    m_ShaderNormTexPos = m_pShader->getShaderSamplerId("normalMap");
    m_ShaderSGITexPos = m_pShader->getShaderSamplerId("specGlossIllMap");
}
void DeferredPreEffect::begin() const
{
    m_pShader->begin();
}
void DeferredPreEffect::end() const
{
    m_pShader->end();
}

void DeferredPreEffect::setWVP(const happyengine::math::Matrix& wvp) const
{
    m_pShader->setShaderVar(m_ShaderWVPpos, wvp);
}
void DeferredPreEffect::setWorld(const happyengine::math::Matrix& world) const
{
    m_pShader->setShaderVar(m_ShaderWorldPos, world);
}
void DeferredPreEffect::setDiffuseMap(const happyengine::graphics::Texture2D::pointer& diffuseMap) const
{
    m_pShader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}
void DeferredPreEffect::setNormalMap(const happyengine::graphics::Texture2D::pointer& normalMap) const
{
    m_pShader->setShaderVar(m_ShaderNormTexPos, normalMap);
}
void DeferredPreEffect::setSpecGlossIllMap(const happyengine::graphics::Texture2D::pointer& sgiMap) const
{
    m_pShader->setShaderVar(m_ShaderSGITexPos, sgiMap);
}

} //end namespace