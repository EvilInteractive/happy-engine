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
//Created: 11/11/2011

#include "StdAfx.h" 

#include "PickEffect.h"
#include "HappyNew.h"

#include <vector>
#include <string>

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
PickEffect::PickEffect() :	m_pShader(nullptr)
{
}

PickEffect::~PickEffect()
{
	delete m_pShader;
}

/* GENERAL */
void PickEffect::load()
{
	ShaderLayout layout;
	layout.addElement(ShaderLayoutElement(0, "inPosition"));

	m_pShader = NEW Shader();

	std::vector<std::string> shaderOutputs;
	shaderOutputs.push_back("outId");

	bool shaderInit(m_pShader->init("../data/shaders/pickingShader.vert", "../data/shaders/pickingShader.frag", layout, shaderOutputs));
	ASSERT(shaderInit == true);

	m_ShaderVPPos = m_pShader->getShaderVarId("matVP");
	m_ShaderWPos = m_pShader->getShaderVarId("matW");
	m_ShaderIDPos = m_pShader->getShaderVarId("id");

	m_pShader->bind();
	m_pShader->setShaderVar(m_ShaderIDPos, (uint)0);
	mat44 MatWVP;
	m_pShader->setShaderVar(m_ShaderVPPos, MatWVP);
}
void PickEffect::begin() const
{
	m_pShader->bind();
}
void PickEffect::end() const
{
}

/* SETTERS */
void PickEffect::setViewProjection(const mat44& mat)
{
	m_pShader->setShaderVar(m_ShaderVPPos, mat);
}
void PickEffect::setWorld(const mat44& mat)
{
	m_pShader->setShaderVar(m_ShaderWPos, mat);
}
void PickEffect::setID(uint id)
{
	m_pShader->setShaderVar(m_ShaderIDPos, (uint)id);
}

} } //end namespace