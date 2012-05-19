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
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "SerializerStream.h"

namespace he {
namespace ge {

SerializerStream::SerializerStream(const std::string& path, bool save)
{
    if (m_Stream.open(path, save?io::BinaryStream::Write:io::BinaryStream::Read) == false)
        HE_ERROR("Error loading save file: %s", path.c_str());
}


SerializerStream::~SerializerStream()
{
}

const SerializerStream& SerializerStream::operator>>(bool& b) const
{
    b = m_Stream.readByte() == 0?false:true;
    return *this;
}
const SerializerStream& SerializerStream::operator>>(byte& b) const
{
    b = m_Stream.readByte();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(ushort& us) const
{
    us = m_Stream.readWord();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(uint& ui) const
{
    ui = m_Stream.readDword();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(float& f) const
{
    f = m_Stream.readFloat();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(double& d) const
{
    d = m_Stream.readDouble();
    return *this;
}

const SerializerStream& SerializerStream::operator>>(vec2& v) const
{
    v = m_Stream.readVector2();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(vec3& v) const
{
    v = m_Stream.readVector3();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(vec4& v) const
{
    v = m_Stream.readVector4();
    return *this;
}

const SerializerStream& SerializerStream::operator>>(mat44& mat) const
{
    mat = m_Stream.readMatrix();
    return *this;
}

const SerializerStream& SerializerStream::operator>>(std::string& str) const
{
    str = m_Stream.readString();
    return *this;
}
const SerializerStream& SerializerStream::operator>>(std::wstring& str) const
{
    str = m_Stream.readWString();
    return *this;
}
//////////////////////////////////////////////////////////////////////////

SerializerStream& SerializerStream::operator<<(bool b)
{
    m_Stream.writeByte(b?1:0);
    return *this;
}
SerializerStream& SerializerStream::operator<<(byte b)
{
    m_Stream.writeByte(b);
    return *this;
}
SerializerStream& SerializerStream::operator<<(ushort us)
{
    m_Stream.writeWord(us);
    return *this;
}
SerializerStream& SerializerStream::operator<<(uint ui)
{
    m_Stream.writeDword(ui);
    return *this;
}
SerializerStream& SerializerStream::operator<<(float f)
{
    m_Stream.writeFloat(f);
    return *this;
}
SerializerStream& SerializerStream::operator<<(double d)
{
    m_Stream.writeDouble(d);
    return *this;
}

SerializerStream& SerializerStream::operator<<(const vec2& v)
{
    m_Stream.writeVector2(v);
    return *this;
}
SerializerStream& SerializerStream::operator<<(const vec3& v)
{
    m_Stream.writeVector3(v);
    return *this;
}
SerializerStream& SerializerStream::operator<<(const vec4& v)
{
    m_Stream.writeVector4(v);
    return *this;
}

SerializerStream& SerializerStream::operator<<(const mat44& mat)
{
    m_Stream.writeMatrix(mat);
    return *this;
}

SerializerStream& SerializerStream::operator<<(const std::string& str)
{
    m_Stream.writeString(str);
    return *this;
}
SerializerStream& SerializerStream::operator<<(const std::wstring& str)
{
    m_Stream.writeWString(str);
    return *this;
}

} } //end namespace