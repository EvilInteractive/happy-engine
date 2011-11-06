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
//Created: 20/08/2011
#include "StdAfx.h" 

#include "BinaryStream.h"
#include "FileNotFoundException.h"
#include "HeAssert.h"

namespace he {
namespace io {

BinaryStream::BinaryStream(const std::string& path, OpenType openType):
    m_pFile(nullptr), m_FileName(path)
{
    errno_t err(fopen_s(&m_pFile, path.c_str(), (openType == Read)? "rb" : "wb"));
    if (err != 0)
    {
        std::cout << "Bin open failed: errno: " << err << "\n";
        throw err::FileNotFoundException(path);
    }
}


BinaryStream::~BinaryStream()
{
    fclose(m_pFile);
}

byte BinaryStream::readByte() const
{
    byte b(0);
    size_t count(fread(&b, sizeof(byte), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return b;
}
ushort BinaryStream::readWord() const
{
    ushort s(0);
    size_t count(fread(&s, sizeof(uint16), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return s;
}
uint BinaryStream::readDword() const
{
    uint i(0);
    size_t count(fread(&i, sizeof(uint32), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return i;
}
float BinaryStream::readFloat() const
{
    float f(0);
    size_t count(fread(&f, sizeof(float), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return f;
}
double BinaryStream::readDouble() const
{
    double d(0);
    size_t count(fread(&d, sizeof(double), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return d;
}
vec2 BinaryStream::readVector2() const
{
    vec2 v;
    readBuffer(&v, sizeof(vec2));
    return v;
}
vec3 BinaryStream::readVector3() const
{
    vec3 v;
    readBuffer(&v, sizeof(vec3));
    return v;
}
vec4 BinaryStream::readVector4() const
{
    vec4 v;
    readBuffer(&v, sizeof(vec4));
    return v;
}
mat44 BinaryStream::readMatrix() const
{
    mat44 m;
    readBuffer(&m, sizeof(mat44));
    return m;
}

std::string BinaryStream::readString() const
{
    uint header(readDword());
    std::string s;
    s.resize(header);
    readBuffer(&s[0], sizeof(char) * header);
    return s;
}
std::wstring BinaryStream::readWString() const
{
    uint header(readDword());
    std::wstring s;
    s.resize(header);
    readBuffer(&s[0], sizeof(wchar_t) * header);
    return s;
}

void BinaryStream::readBuffer(void* buffer, physx::PxU32 size)	const
{
    if (size > 0)
    {
        size_t count(fread(buffer, size, 1, m_pFile));
        ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    }
}

physx::PxStream& BinaryStream::storeByte(byte b)
{
    size_t count(fwrite(&b, sizeof(byte), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
physx::PxStream& BinaryStream::storeWord(ushort w)
{
    size_t count(fwrite(&w, sizeof(uint16), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
physx::PxStream& BinaryStream::storeDword(uint d)
{
    size_t count(fwrite(&d, sizeof(uint32), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
physx::PxStream& BinaryStream::storeFloat(float f)
{
    size_t count(fwrite(&f, sizeof(float), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
physx::PxStream& BinaryStream::storeDouble(double d)
{
    size_t count(fwrite(&d, sizeof(double), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}

physx::PxStream& BinaryStream::storeVector2(const vec2& v)
{
    storeBuffer(&v, sizeof(vec2));
    return *this;
}
physx::PxStream& BinaryStream::storeVector3(const vec3& v)
{
    storeBuffer(&v, sizeof(vec3));
    return *this;
}
physx::PxStream& BinaryStream::storeVector4(const vec4& v)
{
    storeBuffer(&v, sizeof(vec4));
    return *this;
}
physx::PxStream& BinaryStream::storeMatrix(const mat44& m)
{
    storeBuffer(&m, sizeof(mat44));
    return *this;
}

physx::PxStream& BinaryStream::storeString(const std::string& s)
{
    storeDword(s.size());
    storeBuffer(s.c_str(), sizeof(char) * s.size());
    return *this;
}
physx::PxStream& BinaryStream::storeWString(const std::wstring& s)
{
    storeDword(s.size());
    storeBuffer(s.c_str(), sizeof(wchar_t) * s.size());
    return *this;
}

physx::PxStream& BinaryStream::storeBuffer(const void* buffer, physx::PxU32 size)
{
    if (size > 0)
    {
        size_t count(fwrite(buffer, size, 1, m_pFile));
        ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    }
    return *this;
}

} } //end namespace