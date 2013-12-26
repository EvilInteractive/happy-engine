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
//Created: 28/12/2011
#include "HappyPCH.h" 

#include "DynamicBuffer.h"

namespace he {
namespace gfx {

DynamicBuffer::DynamicBuffer(const BufferLayout& bufferLayout): m_Layout(bufferLayout), m_Buffer(nullptr)
{
}


DynamicBuffer::~DynamicBuffer()
{
}

void DynamicBuffer::setValue( uint32 element, float value )
{
    HE_ASSERT(sizeof(float) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(float));
}

void DynamicBuffer::setValue( uint32 element, int value )
{
    HE_ASSERT(sizeof(int) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(int));
}

void DynamicBuffer::setValue( uint32 element, uint32 value )
{
    HE_ASSERT(sizeof(uint32) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(uint32));
}

void DynamicBuffer::setValue( uint32 element, const vec2& value )
{
    HE_ASSERT(sizeof(vec2) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(vec2));
}

void DynamicBuffer::setValue( uint32 element, const vec3& value )
{
    HE_ASSERT(sizeof(vec3) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(vec3));
}

void DynamicBuffer::setValue( uint32 element, const vec4& value )
{
    HE_ASSERT(sizeof(vec4) == m_Layout.getElements()[element].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(vec4));
}

void DynamicBuffer::setValue( uint32 element, const mat44& value )
{
    HE_ASSERT(m_Layout.getElements().size() > element + 3, "No mat44 should be place here");
    HE_ASSERT(sizeof(vec4) == m_Layout.getElements()[element+0].getSize(), "Element is not the right size in bytes!");
    HE_ASSERT(sizeof(vec4) == m_Layout.getElements()[element+1].getSize(), "Element is not the right size in bytes!");
    HE_ASSERT(sizeof(vec4) == m_Layout.getElements()[element+2].getSize(), "Element is not the right size in bytes!");
    HE_ASSERT(sizeof(vec4) == m_Layout.getElements()[element+3].getSize(), "Element is not the right size in bytes!");
    he_memcpy(m_Buffer + m_Layout.getElements()[element].getByteOffset(), &value, sizeof(mat44));
}

const void* DynamicBuffer::getBuffer() const
{
    return m_Buffer;
}

void DynamicBuffer::setBuffer( char* pBuffer )
{
    m_Buffer = pBuffer;
}

} } //end namespace