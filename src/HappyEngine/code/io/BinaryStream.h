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

#ifndef _HE_BINARY_STREAM_H_
#define _HE_BINARY_STREAM_H_
#pragma once

#include "HappyTypes.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat44.h"
#include "common/PxIO.h"


namespace he {
namespace io {

class BinaryStream : public physx::PxOutputStream, public physx::PxInputStream
{
public:
    enum OpenType
    {
        Read, Write
    };
    BinaryStream();
    virtual ~BinaryStream();

    bool open(const std::string& path, OpenType openType);
    void close();

    virtual		byte			readByte()								const;
    virtual		ushort			readWord()								const;
    virtual		uint			readDword()								const;
    virtual		int			    readInt32()								const;
    virtual		float			readFloat()								const;
    virtual		double			readDouble()							const;

    virtual     vec2            readVector2()                           const;
    virtual     vec3            readVector3()                           const;
    virtual     vec4            readVector4()                           const;
    virtual     mat44           readMatrix()                            const;

    virtual     std::string     readString()                            const;
    virtual     std::wstring    readWString()                           const;

    virtual		physx::PxU32	read(void* buffer, physx::PxU32 size)	const;
    virtual		physx::PxU32	read(void* buffer, physx::PxU32 size);

    ////////////////////////////////////////////////////////////////////////////////

    virtual		void		    writeByte(byte b);
    virtual		void		    writeWord(ushort w);
    virtual		void		    writeDword(uint d);
    virtual		void		    writeFloat(float f);
    virtual		void		    writeDouble(double d);

    virtual     void            writeVector2(const vec2& v);
    virtual     void            writeVector3(const vec3& v);
    virtual     void            writeVector4(const vec4& v);
    virtual     void            writeMatrix(const mat44& m);

    virtual     void            writeString(const std::string& s);
    virtual     void            writeWString(const std::wstring& s);

    virtual     physx::PxU32    write(const void* buffer, physx::PxU32 count);

private:
    FILE* m_pFile;
    std::string m_FileName;

    //Disable default copy constructor and default assignment operator
    BinaryStream(const BinaryStream&);
    BinaryStream& operator=(const BinaryStream&);
};

} } //end namespace

#endif
