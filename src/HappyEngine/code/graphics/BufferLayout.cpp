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
#include "HappyPCH.h" 
#include "BufferLayout.h"

namespace he {
namespace gfx {

#pragma region BufferElement
BufferElement::BufferElement(const Type type, const uint32 size, const uint32 byteOffset):
        m_Type(type),
        m_Size(size), m_ByteOffset(byteOffset)
{
    HE_ASSERT(m_Size != 0, "element size == 0");
}

BufferElement::BufferElement():
    m_Type(Type_Int),
    m_Size(0), m_ByteOffset(0)
{

}

uint32 BufferElement::getSize() const
{
    return m_Size;
}
uint32 BufferElement::getByteOffset() const
{
    return m_ByteOffset;
}
BufferElement::Type BufferElement::getType() const
{
    return m_Type;
}
#pragma endregion

#pragma region BufferElement
BufferLayout::BufferLayout(): m_Size(0)
{
}

BufferLayout::BufferLayout(const BufferLayout& other ): m_Size(other.m_Size), m_Layout(other.m_Layout.size())
{
    m_Layout.append(other.m_Layout);
}

BufferLayout& BufferLayout::operator=( const BufferLayout& other )
{
    m_Size = other.m_Size;
    m_Layout.clear();
    m_Layout.reserve(other.m_Layout.size());
    m_Layout.append(other.m_Layout);

    return *this;
}

BufferLayout::~BufferLayout()
{
}

void BufferLayout::addElement(const BufferElement& element)
{
    m_Layout.add(element);
    m_Size += element.getSize();
}

const BufferLayout::layout& BufferLayout::getElements() const
{
    return m_Layout;
}
uint32 BufferLayout::getSize() const
{
    return m_Size;
}
#pragma endregion

} } //end namespace
