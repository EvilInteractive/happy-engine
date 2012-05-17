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

#ifndef _HE_SERIALIZER_STREAM_H_
#define _HE_SERIALIZER_STREAM_H_
#pragma once

#include "BinaryStream.h"

namespace he {
namespace game {

class SerializerStream
{
public:
    SerializerStream(const std::string& path, bool save);
    virtual ~SerializerStream();

    const SerializerStream& operator>>(bool& b) const;
    const SerializerStream& operator>>(byte& b) const;
    const SerializerStream& operator>>(ushort& us) const;
    const SerializerStream& operator>>(uint& ui) const;
    const SerializerStream& operator>>(float& f) const;
    const SerializerStream& operator>>(double& d) const;

    const SerializerStream& operator>>(vec2& v) const;
    const SerializerStream& operator>>(vec3& v) const;
    const SerializerStream& operator>>(vec4& v) const;

    const SerializerStream& operator>>(mat44& mat) const;

    const SerializerStream& operator>>(std::string& str) const;
    const SerializerStream& operator>>(std::wstring& str) const;

    //////////////////////////////////////////////////////////////////////////

    SerializerStream& operator<<(bool b);
    SerializerStream& operator<<(byte b);
    SerializerStream& operator<<(ushort us);
    SerializerStream& operator<<(uint ui);
    SerializerStream& operator<<(float f);
    SerializerStream& operator<<(double d);

    SerializerStream& operator<<(const vec2& v);
    SerializerStream& operator<<(const vec3& v);
    SerializerStream& operator<<(const vec4& v);

    SerializerStream& operator<<(const mat44& mat);

    SerializerStream& operator<<(const std::string& str);
    SerializerStream& operator<<(const std::wstring& str);


private:
    io::BinaryStream m_Stream;

    //Disable default copy constructor and default assignment operator
    SerializerStream(const SerializerStream&);
    SerializerStream& operator=(const SerializerStream&);
};

} } //end namespace

#endif
