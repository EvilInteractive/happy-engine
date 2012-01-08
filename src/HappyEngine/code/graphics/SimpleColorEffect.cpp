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
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "SimpleColorEffect.h"
#include "HappyNew.h"

#include <vector>
#include <string>

#include "ContentManager.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
SimpleColorEffect::SimpleColorEffect() :	m_pShader(nullptr)
{
}

SimpleColorEffect::~SimpleColorEffect()
{
    delete m_pShader;
}

/* GENERAL */
void SimpleColorEffect::load()
{
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    m_pShader = NEW Shader();

    std::vector<std::string> shaderOutputs;
    shaderOutputs.push_back("outColor");

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    bool shaderInit(m_pShader->initFromFile(folder + "2D/simpleShader.vert", 
                                            folder + "2D/simpleShader.frag", layout, shaderOutputs));
    ASSERT(shaderInit == true);

    m_ShaderVPPos = m_pShader->getShaderVarId("matVP");
    m_ShaderWPos = m_pShader->getShaderVarId("matW");
    m_ShaderColorPos = m_pShader->getShaderVarId("color");

    m_pShader->bind();
    m_pShader->setShaderVar(m_ShaderColorPos, vec4(1.0f,1.0f,1.0f,1.0f));
    mat44 MatWVP;// = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_pShader->setShaderVar(m_ShaderVPPos, MatWVP);
}

void SimpleColorEffect::begin() const
{
    m_pShader->bind();
}

void SimpleColorEffect::end() const
{
}

/* SETTERS */
void SimpleColorEffect::setViewProjection(const mat44& mat)
{
    m_pShader->setShaderVar(m_ShaderVPPos, mat);
}

void SimpleColorEffect::setWorld(const mat44& mat)
{
    m_pShader->setShaderVar(m_ShaderWPos, mat);
}

void SimpleColorEffect::setColor(const Color& color)
{
    m_pShader->setShaderVar(m_ShaderColorPos, vec4(color.rgba()));
}

} } //end namespace