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

#include "BinaryFileVisitor.h"

namespace he {
namespace io {

BinaryFileVisitor::BinaryFileVisitor()
    : m_File(nullptr)
    , m_DoEndianSwap(false)
{
}

BinaryFileVisitor::~BinaryFileVisitor()
{
    HE_ASSERT(m_OpenType == BinaryVisitor::eOpenType_Closed, "Stream is still open when disposing BinaryFileVisitor!");
}

bool BinaryFileVisitor::openRead( const Path& path, const bool doBigToLittleEndianSwap )
{
    if (open(path, "rb"))
    {
        m_DoEndianSwap = doBigToLittleEndianSwap;
        m_OpenType = BinaryVisitor::eOpenType_Read;
        return true;
    }
    return false;
}

bool BinaryFileVisitor::openWrite( const Path& path )
{
    if (open(path, "wb"))
    {
        m_OpenType = BinaryVisitor::eOpenType_Write;
        return true;
    }
    return false;
}

bool BinaryFileVisitor::open( const Path& path, const char* type )
{
    m_File = std::fopen(path.str().c_str(), type);
    if (m_File == nullptr)
    {
        HE_ERROR("BinaryFileVisitor open file failed('%s'): errno: %d", path.str().c_str(), std::ferror(m_File));
        return false;
    }
    return true;
}

void BinaryFileVisitor::close()
{
    HE_ASSERT(m_OpenType != BinaryVisitor::eOpenType_Closed, "BinaryFileVisitor not open!");
    m_OpenType = BinaryVisitor::eOpenType_Closed;
    if (m_File != nullptr)
    {
        std::fclose(m_File);
        m_File = nullptr;
    }
}

size_t BinaryFileVisitor::readBuffer( void* buffer, const size_t size )
{
    if (size > 0)
    {
        const size_t count(std::fread(buffer, size, 1, m_File));
        if (m_DoEndianSwap)
        {
            char* value(static_cast<char*>(buffer));
            std::reverse(value, value + size);
        }

        HE_ASSERT(count == 1, "unsuccessful read operation!"); count;
    }
    return size;
}

size_t BinaryFileVisitor::writeBuffer(const void* buffer, const size_t size)
{
    if (size > 0)
    {
        const size_t count(fwrite(buffer, size, 1, m_File));
        HE_ASSERT(count == 1, "unsuccessful write operation!"); count;
    }
    return size;
}

void BinaryFileVisitor::skipBytes( const size_t bytes )
{
    const size_t processedBytes(getProcessedBytes());
    int ret(std::fseek(m_File, bytes, SEEK_CUR));
    HE_ASSERT(ret == 0, "Error occurred when skipping bytes in file");
    HE_ASSERT(processedBytes + bytes == getProcessedBytes(), "I did not skip the requested bytes! Got file pointer %d instead of %d", getProcessedBytes(), processedBytes + bytes);
}

size_t BinaryFileVisitor::getProcessedBytes()
{
    return std::ftell(m_File);
}

} } //end namespace
