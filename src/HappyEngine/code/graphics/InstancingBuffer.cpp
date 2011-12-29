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
//Created: 28/12/2011
#include "HappyPCH.h" 

#include "InstancingBuffer.h"

namespace he {
namespace gfx {
namespace details {

InstancingBuffer::InstancingBuffer(uint itemSize, uint maxItems): 
            m_Size(itemSize * maxItems), m_Count(0), m_ItemSize(itemSize)
{
    m_Buffer = static_cast<char*>(malloc(m_Size));
    m_End = m_Buffer;
}


InstancingBuffer::~InstancingBuffer()
{
    free(m_Buffer);
}

void InstancingBuffer::resize( uint newSize )
{
    if (m_Size != newSize)
    {
        m_Size = newSize;
        m_Buffer = static_cast<char*>(realloc(m_Buffer, newSize));
        ASSERT(m_Buffer != nullptr, "realloc failed - out of memory?");
        reset();
    }
}

char* InstancingBuffer::addItem()
{
    ASSERT(getCount() < getCountCapacity(), "Buffer overrun! resize buffer!");
    ++m_Count;
    m_End += m_ItemSize;
    return m_End - m_ItemSize;
}
void InstancingBuffer::reset()
{
    m_End = m_Buffer;
    m_Count = 0;
}

uint InstancingBuffer::getSize() const
{
    return m_Count * m_ItemSize;
}
uint InstancingBuffer::getSizeCapacity() const
{
    return m_Size;
}

uint InstancingBuffer::getCount() const
{
    return m_Count;
}
uint InstancingBuffer::getCountCapacity() const
{
    return m_Size / m_ItemSize;
}

char* InstancingBuffer::getBuffer() const
{
    return m_Buffer;
}



} } } //end namespace