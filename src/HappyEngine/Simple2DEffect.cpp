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

#include "Simple2DEffect.h"
#include "HappyNew.h"
#include "VertexLayout.h"
#include "Assert.h"

namespace happyengine {
namespace graphics {

Simple2DEffect::Simple2DEffect() : m_pShader(nullptr)
{
}


Simple2DEffect::~Simple2DEffect()
{
	delete m_pShader;
}

void Simple2DEffect::load()
{
	using namespace happyengine;
	using namespace graphics;

	VertexLayout layout;
	layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0, "inPosition"));
	layout.addElement(VertexElement(1, VertexElement::Type_Vector3, VertexElement::Usage_Other, 12, 12, "inColor"));

	m_pShader = NEW Shader();
	std::vector<std::string> shaderOutputs;
	shaderOutputs.push_back("outColor");
	ASSERT(m_pShader->init("../data/shaders/simpleShader.vert", "../data/shaders/simpleShader.frag", layout, shaderOutputs) == false);

	m_ShaderWVPPos = m_pShader->getShaderVarId("matWVP");

	math::Matrix MatWVP = math::Matrix::createTranslation(math::Vector3(0.0f,0.0f,0.0f));
	m_pShader->setShaderVar(m_ShaderWVPPos, MatWVP);
}

void Simple2DEffect::begin() const
{
	m_pShader->begin();
}

void Simple2DEffect::end() const
{
	m_pShader->end();
}

} } //end namespace