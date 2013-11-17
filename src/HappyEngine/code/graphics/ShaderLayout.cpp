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



ShaderLayoutElement::ShaderLayoutElement(uint32 elementIndex, const he::String& nameInShader): m_ElementIndex(elementIndex), m_NameInShader(nameInShader)
{
}

uint32 ShaderLayoutElement::getElementIndex() const
{
    return m_ElementIndex;
}
const he::String& ShaderLayoutElement::getShaderVariableName() const
{
    return m_NameInShader;
}



ShaderLayout::ShaderLayout()
{
}

ShaderLayout::~ShaderLayout()
{
}

void ShaderLayout::addElement(const ShaderLayoutElement& element)
{
    m_Layout.add(element);
}

const ShaderLayout::layout& ShaderLayout::getElements() const
{
    return m_Layout;
}

} } //end namespace