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
//Author: Bastian Damman
//Created: 01/10/2011
#include "HappyPCH.h" 

#include "ShaderLayout.h"

namespace he {
namespace gfx {

ShaderLayoutAttribute::ShaderLayoutAttribute(const he::FixedString& nameInShader, const EShaderAttributePropertyUsage usage, const uint32 index)
    : m_ElementIndex(index)
    , m_Usage(usage)
    , m_Name(nameInShader)
{
}

ShaderLayout::ShaderLayout()
{
}

ShaderLayout::~ShaderLayout()
{
}

void ShaderLayout::addAttribute(const ShaderLayoutAttribute& element)
{
    m_Attributes.add(element);
}

void ShaderLayout::addUniform( const ShaderLayoutUniform& element )
{
    m_Uniforms.add(element);
}

} } //end namespace
