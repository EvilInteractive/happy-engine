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
//Created: 26/01/2013
#include "HappyPCH.h"

#include "BinaryStreamVisitor.h"

namespace he {
namespace io {

BinaryStreamVisitor::BinaryStreamVisitor()
    : m_ReadIndex(0)
{
}

BinaryStreamVisitor::~BinaryStreamVisitor()
{
    HE_ASSERT(m_OpenType == BinaryVisitor::eOpenType_Closed, "Stream is still open when disposing BinaryStreamVisitor!");
}

void BinaryStreamVisitor::openWrite()
{
    HE_ASSERT(m_OpenType == BinaryVisitor::eOpenType_Closed, "Stream already open!");
    m_OpenType = BinaryVisitor::eOpenType_Write;
    m_ReadIndex = 0;
    m_Data.clear();
}

void BinaryStreamVisitor::openRead()
{
    HE_ASSERT(m_OpenType == BinaryVisitor::eOpenType_Closed, "Stream already open!");
    m_OpenType = BinaryVisitor::eOpenType_Read;
    m_ReadIndex = 0;
}

void BinaryStreamVisitor::close()
{
    HE_ASSERT(m_OpenType != BinaryVisitor::eOpenType_Closed, "Stream already closed!");
    m_OpenType = BinaryVisitor::eOpenType_Closed;
    m_Data.trim();
    m_ReadIndex = 0;
}

const uint8* BinaryStreamVisitor::getData() const
{
    return &m_Data[0];
}

size_t BinaryStreamVisitor::getByteCount() const
{
    return m_Data.size();
}

size_t BinaryStreamVisitor::readBuffer( void* buffer, const size_t byteCount )
{
    memcpy(buffer, &m_Data[m_ReadIndex], byteCount);
    m_ReadIndex += byteCount;
    return byteCount;
}

size_t BinaryStreamVisitor::writeBuffer( const void* buffer, const size_t byteCount ) 
{
    m_Data.resize(m_Data.size() + byteCount);
    memcpy(&m_Data[m_ReadIndex], buffer, byteCount);
    m_ReadIndex += byteCount;
    return byteCount;
}

void BinaryStreamVisitor::skipBytes( const size_t bytes )
{
    HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
    HE_ASSERT(m_OpenType == eOpenType_Read, "Stream can only skip bytes when reading!");
    m_ReadIndex += bytes;
}

size_t BinaryStreamVisitor::getProcessedBytes()
{
    return m_ReadIndex;
}

} } //end namespace
