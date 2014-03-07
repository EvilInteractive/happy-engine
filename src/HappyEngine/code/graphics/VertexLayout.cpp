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
//Author:  Bastian Damman
//Created: 2014/03/07
#include "HappyPCH.h" 
#include "VertexLayout.h"

namespace he {
namespace gfx {

#pragma region BufferElement

VertexElement::VertexElement( const EShaderAttribute attrib, const EShaderAttributeType type, const EShaderAttributeTypeComponents components, const uint32 byteOffset )
    : m_Type(type)
    , m_Components(components)
    , m_Attribute(attrib)
    , m_ByteOffset(byteOffset)
{
    HE_ASSERT(getShaderAttributeSize(m_Type, m_Components) != 0, "element size == 0");
}

VertexElement::VertexElement()
    : m_Type(eShaderAttributeType_Float)
    , m_Components(eShaderAttributeTypeComponents_1)
    , m_Attribute(eShaderAttribute_Invalid)
    , m_ByteOffset(0)
{

}

#pragma endregion

#pragma region BufferElement
VertexLayout::VertexLayout(): m_Size(0)
{
}

VertexLayout::VertexLayout(const VertexLayout& other ): m_Size(other.m_Size), m_Layout(other.m_Layout.size())
{
    m_Layout.append(other.m_Layout);
}

VertexLayout& VertexLayout::operator=( const VertexLayout& other )
{
    m_Size = other.m_Size;
    m_Layout.clear();
    m_Layout.reserve(other.m_Layout.size());
    m_Layout.append(other.m_Layout);

    return *this;
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::addElement(const VertexElement& element)
{
    m_Layout.add(element);
    m_Size += element.getSize();
}

const VertexLayout::layout& VertexLayout::getElements() const
{
    return m_Layout;
}
uint32 VertexLayout::getSize() const
{
    return m_Size;
}
#pragma endregion

} } //end namespace
