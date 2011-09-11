//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Simple2DTextureEffect.h"
#include "HappyNew.h"
#include "VertexLayout.h"
#include "Assert.h"

namespace happyengine {
namespace graphics {

Simple2DTextureEffect::Simple2DTextureEffect() : m_pShader(nullptr)
{
}


Simple2DTextureEffect::~Simple2DTextureEffect()
{
	delete m_pShader;
}

void Simple2DTextureEffect::load()
{
	using namespace happyengine;
	using namespace graphics;

	VertexLayout layout;
	layout.addElement(VertexElement(0, VertexElement::Type_Vector2, VertexElement::Usage_Position, 8, 0, "inPosition"));
	layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, 8, 8, "inTexCoord"));

	m_pShader = NEW Shader();
	std::vector<std::string> shaderOutputs;
	shaderOutputs.push_back("outColor");
	ASSERT(m_pShader->init("../data/shaders/simple2DTextureShader.vert", "../data/shaders/simple2DTextureShader.frag", layout, shaderOutputs) == true);

	m_ShaderWVPPos = m_pShader->getShaderVarId("matWVP");
	m_ShaderDiffTexPos = m_pShader->getShaderSamplerId("diffuseMap");
	m_ShaderAlphaPos = m_pShader->getShaderVarId("inAlpha");
	
	m_pShader->begin();
	math::Matrix MatWVP = math::Matrix::createTranslation(math::Vector3(0.0f,0.0f,0.0f));
	m_pShader->setShaderVar(m_ShaderWVPPos, MatWVP);
    m_pShader->end();
}

void Simple2DTextureEffect::begin() const
{
	m_pShader->begin();
}

void Simple2DTextureEffect::end() const
{
	m_pShader->end();
}

void Simple2DTextureEffect::setWorldMatrix(const happyengine::math::Matrix& mat) const
{
	m_pShader->setShaderVar(m_ShaderWVPPos, mat);
}

void Simple2DTextureEffect::setDiffuseMap(const happyengine::graphics::Texture2D::pointer& diffuseMap) const
{
	m_pShader->setShaderVar(m_ShaderDiffTexPos, diffuseMap);
}

void Simple2DTextureEffect::setAlpha(const float alpha) const
{
	m_pShader->setShaderVar(m_ShaderAlphaPos, alpha);
}

} } //end namespace