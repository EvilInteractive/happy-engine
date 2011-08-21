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

#include "BinaryStream.h"
#include "FileNotFoundException.h"
#include "Assert.h"

namespace happyengine {
namespace io {

BinaryStream::BinaryStream(const std::string& path, OpenType openType):
    m_pFile(nullptr), m_FileName(path)
{
    errno_t error(fopen_s(&m_pFile, path.c_str(), (openType == Read)? "rb" : "wb"));
    if (error != 0)
        throw error::FileNotFoundException(path);
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
    size_t count(fread(&s, sizeof(ushort), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
    return s;
}
uint BinaryStream::readDword() const
{
    uint i(0);
    size_t count(fread(&i, sizeof(uint), 1, m_pFile));
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
math::Vector2 BinaryStream::readVector2() const
{
    math::Vector2 v;
    readBuffer(&v, sizeof(math::Vector2));
    return v;
}
math::Vector3 BinaryStream::readVector3() const
{
    math::Vector3 v;
    readBuffer(&v, sizeof(math::Vector3));
    return v;
}
math::Vector4 BinaryStream::readVector4() const
{
    math::Vector4 v;
    readBuffer(&v, sizeof(math::Vector4));
    return v;
}
math::Matrix BinaryStream::readMatrix() const
{
    math::Matrix m;
    readBuffer(&m, sizeof(math::Matrix));
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

void BinaryStream::readBuffer(void* buffer, PxU32 size)	const
{
    size_t count(fread(buffer, size, 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull read operation in file: " + m_FileName).c_str());
}

PxStream& BinaryStream::storeByte(byte b)
{
    size_t count(fwrite(&b, sizeof(byte), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
PxStream& BinaryStream::storeWord(ushort w)
{
    size_t count(fwrite(&w, sizeof(ushort), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
PxStream& BinaryStream::storeDword(uint d)
{
    size_t count(fwrite(&d, sizeof(uint), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
PxStream& BinaryStream::storeFloat(float f)
{
    size_t count(fwrite(&f, sizeof(float), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}
PxStream& BinaryStream::storeDouble(double d)
{
    size_t count(fwrite(&d, sizeof(double), 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}

PxStream& BinaryStream::storeVector2(const math::Vector2& v)
{
    storeBuffer(&v, sizeof(math::Vector2));
    return *this;
}
PxStream& BinaryStream::storeVector3(const math::Vector3& v)
{
    storeBuffer(&v, sizeof(math::Vector3));
    return *this;
}
PxStream& BinaryStream::storeVector4(const math::Vector4& v)
{
    storeBuffer(&v, sizeof(math::Vector4));
    return *this;
}
PxStream& BinaryStream::storeMatrix(const math::Matrix& m)
{
    storeBuffer(&m, sizeof(math::Matrix));
    return *this;
}

PxStream& BinaryStream::storeString(const std::string& s)
{
    storeDword(s.size());
    storeBuffer(s.c_str(), sizeof(char) * s.size());
    return *this;
}
PxStream& BinaryStream::storeWString(const std::wstring& s)
{
    storeDword(s.size());
    storeBuffer(s.c_str(), sizeof(wchar_t) * s.size());
    return *this;
}

PxStream& BinaryStream::storeBuffer(const void* buffer, PxU32 size)
{
    size_t count(fwrite(buffer, size, 1, m_pFile));
    ASSERT(count == 1, ("unsuccesfull write operation in file: " + m_FileName).c_str());
    return *this;
}

} } //end namespace