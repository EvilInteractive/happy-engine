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

#include "HappyPCH.h" 

#include "PickEffect.h"
#include "HappyNew.h"

#include "Shader.h"

#include "ContentManager.h"

#include <vector>
#include <string>

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
PickEffect::PickEffect()
{
}

PickEffect::~PickEffect()
{
}

/* GENERAL */
void PickEffect::load()
{
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    BufferLayout vertexLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

    Shader::pointer pShader(NEW Shader());

    //std::vector<std::string> shaderOutputs;
    ////shaderOutputs.push_back("outId");

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    bool shaderInit(pShader->initFromFile(folder + "2D/pickingShader.vert", 
                                          folder + "2D/pickingShader.frag", layout));
    ASSERT(shaderInit == true);

    m_PickMaterial.setShader(pShader, vertexLayout, false);

    m_PickMaterial.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShader->getShaderVarId("matVP"), ShaderVarType_ViewProjection)));
    m_PickMaterial.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShader->getShaderVarId("matW"), ShaderVarType_World)));

    m_IdVar = ShaderUserVar<vec4>::pointer(NEW ShaderUserVar<vec4>(pShader->getShaderVarId("id"), vec4(0, 0, 0, 0)));
    m_PickMaterial.addVar(m_IdVar);
}

/* SETTERS */
void PickEffect::setID(const vec4& id)
{
    dynamic_cast<ShaderUserVar<vec4>*>(m_IdVar.get())->setData(id); // TODO change this ugly line
}

const Material& PickEffect::getMaterial() const
{
    return m_PickMaterial;
}

} } //end namespace