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
//Created: 11/11/2011

#include "HappyPCH.h" 

#include "PickEffect.h"

#include "ShaderVar.h"
#include "Shader.h"
#include "Material.h"

#include "ContentManager.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
PickEffect::PickEffect(): m_PickMaterial(nullptr)
{
}

PickEffect::~PickEffect()
{
    if (m_PickMaterial != nullptr)
        m_PickMaterial->release();
}

/* GENERAL */
void PickEffect::load()
{
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    BufferLayout vertexLayout, instancingLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

    Shader* shader(ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create()));
    
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    const bool shaderInit(shader->initFromFile(folder + "2D/pickingShader.vert", 
                                         folder + "2D/pickingShader.frag", layout));
    HE_ASSERT(shaderInit == true, "picking shader init failed"); shaderInit;

    m_PickMaterial = ResourceFactory<Material>::getInstance()->get(ResourceFactory<Material>::getInstance()->create());
    m_PickMaterial->setShader(shader->getHandle(), vertexLayout, instancingLayout);

    m_PickMaterial->registerVar(NEW ShaderGlobalVar(shader->getShaderVarId("matWVP"), "matVP", ShaderVarType_WorldViewProjection));

    m_IdVar = NEW ShaderUserVar<vec4>(shader->getShaderVarId("id"), "id", vec4(0, 0, 0, 0));
    m_PickMaterial->registerVar(m_IdVar);

    shader->release();
}

/* SETTERS */
void PickEffect::setID(const vec4& id)
{
    m_IdVar->setData(id);
}

const Material* PickEffect::getMaterial() const
{
    return m_PickMaterial;
}

} } //end namespace