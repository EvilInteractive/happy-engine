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
#include "HappyPCH.h" 

#include "VertexLayout.h"
#include "HeAssert.h"

namespace he {
namespace gfx {

#pragma region VertexElement
VertexElement::VertexElement(uint elementIndex, Type type, Usage usage, uint size, uint byteOffset):
        m_ElementIndex(elementIndex), m_Type(type), m_Usage(usage),
        m_Size(size), m_ByteOffset(byteOffset)
{
    ASSERT(m_Size != 0, "element size == 0");
}
uint VertexElement::getElementIndex() const
{
    return m_ElementIndex;
}
uint VertexElement::getSize() const
{
    return m_Size;
}
uint VertexElement::getByteOffset() const
{
    return m_ByteOffset;
}
VertexElement::Type VertexElement::getType() const
{
    return m_Type;
}
VertexElement::Usage VertexElement::getUsage() const
{
    return m_Usage;
}
#pragma endregion

#pragma region VertexLayout
VertexLayout::VertexLayout(): m_VertexSize(0)
{
}
VertexLayout::~VertexLayout()
{
}

void VertexLayout::addElement(const VertexElement& element)
{
    m_Layout.push_back(element);
    m_VertexSize += element.getSize();
}

const VertexLayout::layout& VertexLayout::getElements() const
{
    return m_Layout;
}
uint VertexLayout::getVertexSize() const
{
    ASSERT(m_VertexSize != 0, "Vertex size == 0!");
    return m_VertexSize;
}
#pragma endregion

} } //end namespace