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
//Created: 20/08/2011
#include "HappyPCH.h"

#include "BinaryStream.h"

namespace he {
namespace io {

BinaryStream::BinaryStream():
    m_pFile(nullptr), m_FileName("")
{
}
bool BinaryStream::open( const std::string& path, OpenType openType )
{
    m_FileName = path;

    int err(0);
#if !GCC && !LLVM
    err = fopen_s(&m_pFile, path.c_str(), (openType == Read)? "rb" : "wb");
#else
    m_pFile = fopen(path.c_str(), (openType == Read)? "rb" : "wb");
    if (m_pFile == nullptr)
        err = 1;
    else
        err = 0;
#endif
    if (err != 0)
    {
        HE_ERROR("Bin open failed('%s'): errno: %d", path.c_str(), err);
        return false;
    }
    return true;
}

BinaryStream::~BinaryStream()
{
    close();
}
void BinaryStream::close()
{
    if (m_pFile != nullptr)
    {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}


uint8 BinaryStream::readByte() const
{
    uint8 b(0);
    size_t count(fread(&b, sizeof(uint8), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return b;
}
uint16 BinaryStream::readWord() const
{
    uint16 s(0);
    size_t count(fread(&s, sizeof(uint16), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return s;
}
uint32 BinaryStream::readDword() const
{
    uint32 i(0);
    size_t count(fread(&i, sizeof(uint32), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return i;
}
int BinaryStream::readInt32() const
{
    int i(0);
    size_t count(fread(&i, sizeof(int32), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return i;
}
float BinaryStream::readFloat() const
{
    float f(0);
    size_t count(fread(&f, sizeof(float), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return f;
}
double BinaryStream::readDouble() const
{
    double d(0);
    size_t count(fread(&d, sizeof(double), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccesfull read operation in file: %s", m_FileName.c_str());
    return d;
}
vec2 BinaryStream::readVector2() const
{
    vec2 v;
    read(&v, sizeof(vec2));
    return v;
}
vec3 BinaryStream::readVector3() const
{
    vec3 v;
    read(&v, sizeof(vec3));
    return v;
}
vec4 BinaryStream::readVector4() const
{
    vec4 v;
    read(&v, sizeof(vec4));
    return v;
}
mat44 BinaryStream::readMatrix() const
{
    mat44 m;
    read(&m, sizeof(mat44));
    return m;
}

std::string BinaryStream::readString() const
{
    uint32 header(readDword());
    std::string s;
    s.resize(header);
    read(&s[0], sizeof(char) * header);
    return s;
}
std::wstring BinaryStream::readWString() const
{
    uint32 header(readDword());
    std::wstring s;
    s.resize(header);
    read(&s[0], sizeof(wchar_t) * header);
    return s;
}

physx::PxU32 BinaryStream::read(void* buffer, physx::PxU32 size)	const
{
    if (size > 0)
    {
        size_t count(fread(buffer, size, 1, m_pFile));
        HE_ASSERT(count == 1, "unsuccessful read operation in file: %s", m_FileName.c_str());
    }
    return size;
}

physx::PxU32 BinaryStream::read( void* buffer, physx::PxU32 size )
{
    if (size > 0)
    {
        size_t count(fread(buffer, size, 1, m_pFile));
        HE_ASSERT(count == 1, "unsuccessful read operation in file: %s", m_FileName.c_str());
    }
    return size;
}

void BinaryStream::writeByte(uint8 b)
{
    size_t count(fwrite(&b, sizeof(uint8), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
}
void BinaryStream::writeWord(uint16 w)
{
    size_t count(fwrite(&w, sizeof(uint16), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
}
void BinaryStream::writeDword(uint32 d)
{
    size_t count(fwrite(&d, sizeof(uint32), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
}
void BinaryStream::writeFloat(float f)
{
    size_t count(fwrite(&f, sizeof(float), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
}
void BinaryStream::writeDouble(double d)
{
    size_t count(fwrite(&d, sizeof(double), 1, m_pFile));
    HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
}

void BinaryStream::writeVector2(const vec2& v)
{
    write(&v, sizeof(vec2));
}
void BinaryStream::writeVector3(const vec3& v)
{
    write(&v, sizeof(vec3));
}
void BinaryStream::writeVector4(const vec4& v)
{
    write(&v, sizeof(vec4));
}
void BinaryStream::writeMatrix(const mat44& m)
{
    write(&m, sizeof(mat44));
}

void BinaryStream::writeString(const std::string& s)
{
    writeDword(s.size());
    write(s.c_str(), sizeof(char) * s.size());
}
void BinaryStream::writeWString(const std::wstring& s)
{
    writeDword(s.size());
    write(s.c_str(), sizeof(wchar_t) * s.size());
}

physx::PxU32 BinaryStream::write(const void* buffer, physx::PxU32 size)
{
    if (size > 0)
    {
        size_t count(fwrite(buffer, size, 1, m_pFile));
        HE_ASSERT(count == 1, "unsuccessful write operation in file: %s", m_FileName.c_str());
    }
    return size;
}


} } //end namespace
