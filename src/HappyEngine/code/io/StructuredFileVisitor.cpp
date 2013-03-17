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
//Created: 26/01/2013
#include "HappyPCH.h"

#include "StructuredFileVisitor.h"

#include <json/json.h>

namespace he {
namespace io {

StructuredFileVisitor::StructuredFileVisitor()
    : m_File(nullptr)
{
}

StructuredFileVisitor::~StructuredFileVisitor()
{
    HE_ASSERT(m_OpenType == BinaryVisitor::eOpenType_Closed, "Stream is still open when disposing StructuredFileVisitor!");
}

bool StructuredFileVisitor::openRead( const Path& path )
{
    
    if (open(path, "rb"))
    {
        m_OpenType = BinaryVisitor::eOpenType_Read;
        return true;
    }
    return false;
}

bool StructuredFileVisitor::openWrite( const Path& path )
{
    if (open(path, "wb"))
    {
        m_OpenType = BinaryVisitor::eOpenType_Write;
        return true;
    }
    return false;
}

bool StructuredFileVisitor::open( const Path& path, const char* type )
{
    int err(0);
#ifdef _MSC_VER
    err = fopen_s(&m_File, path.str().c_str(), type);
#else
    m_File = fopen(path.str().c_str(), type);
    if (m_File == nullptr)
        err = 1;
    else
        err = 0;
#endif
    if (err != 0)
    {
        HE_ERROR("StructuredFileVisitor open file failed('%s'): errno: %d", path.str().c_str(), err);
        return false;
    }
    return true;
}

void StructuredFileVisitor::close()
{
    HE_ASSERT(m_OpenType != BinaryVisitor::eOpenType_Closed, "StructuredFileVisitor not open!");
    m_OpenType = BinaryVisitor::eOpenType_Closed;
    if (m_File != nullptr)
    {
        fclose(m_File);
        m_File = nullptr;
    }
}

size_t StructuredFileVisitor::readBuffer( void* buffer, const size_t size )
{
    if (size > 0)
    {
        const size_t count(fread(buffer, size, 1, m_File));
        HE_ASSERT(count == 1, "unsuccessful read operation!"); count;
    }
    return size;
}

size_t StructuredFileVisitor::writeBuffer(const void* buffer, const size_t size)
{
    if (size > 0)
    {
        const size_t count(fwrite(buffer, size, 1, m_File));
        HE_ASSERT(count == 1, "unsuccessful write operation!"); count;
    }
    return size;
}

} } //end namespace
