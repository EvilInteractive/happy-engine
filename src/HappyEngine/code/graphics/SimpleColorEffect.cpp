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

#include "ContentManager.h"

#include "Shader.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
SimpleColorEffect::SimpleColorEffect() : m_Shader(nullptr)
{
}

SimpleColorEffect::~SimpleColorEffect()
{
    if (m_Shader != nullptr)
        m_Shader->release();
}

/* GENERAL */
void SimpleColorEffect::load()
{
    ShaderLayout layout;
    layout.addAttribute(ShaderLayoutAttribute(0, "inPosition"));

    m_Shader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());

    he::ObjectList<he::String> shaderOutputs;
    shaderOutputs.add("outColor");

    const he::String& folder(CONTENT->getShaderFolderPath().str());
    const bool shaderInit(m_Shader->initFromFile(folder + "2D/simpleShader.vert", 
                                            folder + "2D/simpleShader.frag", layout, shaderOutputs));
    HE_ASSERT(shaderInit == true, "simpleShader init failed"); shaderInit;

    m_ShaderVPPos = m_Shader->getShaderVarId("matVP");
    m_ShaderWPos = m_Shader->getShaderVarId("matW");
    m_ShaderColorPos = m_Shader->getShaderVarId("color");

    m_Shader->bind();
    m_Shader->setShaderVar(m_ShaderColorPos, vec4(1.0f,1.0f,1.0f,1.0f));
    mat44 MatWVP;// = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_Shader->setShaderVar(m_ShaderVPPos, MatWVP);
}

void SimpleColorEffect::begin() const
{
    m_Shader->bind();
}

void SimpleColorEffect::end() const
{
}

/* SETTERS */
void SimpleColorEffect::setViewProjection(const mat44& mat)
{
    m_Shader->setShaderVar(m_ShaderVPPos, mat);
}

void SimpleColorEffect::setWorld(const mat44& mat)
{
    m_Shader->setShaderVar(m_ShaderWPos, mat);
}

void SimpleColorEffect::setColor(const Color& color)
{
    m_Shader->setShaderVar(m_ShaderColorPos, vec4(color.rgba()));
}

} } //end namespace