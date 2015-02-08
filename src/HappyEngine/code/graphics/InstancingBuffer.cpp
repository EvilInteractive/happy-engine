//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

InstancingBuffer::InstancingBuffer(uint32 itemSize, uint32 maxItems): 
            m_Size(itemSize * maxItems), m_Count(0), m_ItemSize(itemSize)
{
    m_Buffer = static_cast<char*>(he_malloc("InstancingBuffer::m_Buffer", m_Size));
    m_End = m_Buffer;
}

InstancingBuffer::InstancingBuffer(): 
            m_Size(0), m_Count(0), m_ItemSize(0), m_Buffer(nullptr), m_End(nullptr)
{

}

InstancingBuffer& InstancingBuffer::operator=( const InstancingBuffer& other )
{
    he_free(m_Buffer);

    m_Size = other.m_Size;
    m_ItemSize = other.m_ItemSize;
    m_Count = other.m_Count;

    m_Buffer = static_cast<char*>(he_malloc("InstancingBuffer::m_Buffer", m_Size));
    m_End = m_Buffer;

    return *this;
}



InstancingBuffer::~InstancingBuffer()
{
    he_free(m_Buffer);
}

void InstancingBuffer::resize( uint32 newSize )
{
    if (m_Size != newSize)
    {
        char* newBuffer(static_cast<char*>(he_realloc("InstancingBuffer::m_Buffer", m_Buffer, newSize)));
        HE_ASSERT(newBuffer != nullptr, "realloc failed - out of memory?");
        if (newBuffer != nullptr)
        {
            m_Buffer = newBuffer;
            m_Size = newSize;
        }
        reset();
    }
}

char* InstancingBuffer::addItem()
{
    HE_ASSERT(getCount() < getCountCapacity(), "Buffer overrun! resize buffer!");
    ++m_Count;
    m_End += m_ItemSize;
    return m_End - m_ItemSize;
}
void InstancingBuffer::reset()
{
    m_End = m_Buffer;
    m_Count = 0;
}

uint32 InstancingBuffer::getSize() const
{
    return m_Count * m_ItemSize;
}
uint32 InstancingBuffer::getSizeCapacity() const
{
    return m_Size;
}

uint32 InstancingBuffer::getCount() const
{
    return m_Count;
}
uint32 InstancingBuffer::getCountCapacity() const
{
    return m_Size / m_ItemSize;
}

char* InstancingBuffer::getBuffer() const
{
    return m_Buffer;
}



} } } //end namespace