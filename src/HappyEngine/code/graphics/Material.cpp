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
//Author:  Bastian Damman
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "Material.h"
#include "HappyNew.h"
#include "IDrawable.h"

namespace he {
namespace gfx {

Material::Material(): m_UsedForInstancing(false), m_IsTranslucent(false)
{
}


Material::~Material()
{
}

void Material::addVar(const ShaderVar::pointer& var)
{
    ASSERT(!m_UsedForInstancing || (var->getType() == ShaderVarType_ViewProjection || var->getType() == ShaderVarType_User), "ShaderVarType not supported for instancing");
    m_ShaderVar.push_back(var);
}
void Material::setShader(const Shader::pointer& pShader, const VertexLayout& compatibleVL, bool usedForInstancing)
{
    m_UsedForInstancing = usedForInstancing;
    m_pShader = pShader;
    m_CompatibleVL = compatibleVL;
}

void Material::apply(const IDrawable* pDrawable, const Camera* pCamera) const
{
    ASSERT(m_pShader != nullptr, "set shader first!");
    m_pShader->bind();
    std::for_each(m_ShaderVar.cbegin(), m_ShaderVar.cend(), [&](const ShaderVar::pointer& pVar)
    {
        if (pVar->getType() == ShaderVarType_User)
        {
            pVar->assignData(m_pShader);
        }
        else
        {
            switch (pVar->getType())
            {      
                case ShaderVarType_WorldViewProjection: m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection() * pDrawable->getWorldMatrix()); break;
                case ShaderVarType_ViewProjection: m_pShader->setShaderVar(pVar->getId(), pCamera->getViewProjection()); break;
                case ShaderVarType_World: m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); break;
                case ShaderVarType_WorldView: m_pShader->setShaderVar(pVar->getId(), pCamera->getView() * pDrawable->getWorldMatrix()); break;

                case ShaderVarType_BoneTransforms: m_pShader->setShaderVar(pVar->getId(), pDrawable->getBoneTransforms()); break;

                case ShaderVarType_WorldPosition: m_pShader->setShaderVar(pVar->getId(), pDrawable->getWorldMatrix().getTranslation()); break;

                default: ASSERT(false, "unkown shaderVartype"); break;
            }
        }
    });
}

const VertexLayout& Material::getCompatibleVertexLayout() const
{
    return m_CompatibleVL;
}

} } //end namespace