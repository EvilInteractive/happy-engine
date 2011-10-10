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

#ifndef _HE_BINARY_STREAM_H_
#define _HE_BINARY_STREAM_H_
#pragma once

#include "common/PxStream.h"
#include "HappyTypes.h"

#include <string>
#include <iostream>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat44.h"

namespace he {
namespace io {

class BinaryStream : public PxStream
{
public:
    enum OpenType
    {
        Read, Write
    };
	BinaryStream(const std::string& path, OpenType openType);
    virtual ~BinaryStream();

    virtual		byte			readByte()								const;
	virtual		ushort			readWord()								const;
	virtual		uint			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;

    virtual     vec2   readVector2()                           const;
    virtual     vec3   readVector3()                           const;
    virtual     vec4   readVector4()                           const;
    virtual     mat44    readMatrix()                            const;

    virtual     std::string     readString()                            const;
    virtual     std::wstring    readWString()                           const;

	virtual		void			readBuffer(void* buffer, PxU32 size)	const;

	virtual		PxStream&		storeByte(byte b);
	virtual		PxStream&		storeWord(ushort w);
	virtual		PxStream&		storeDword(uint d);
	virtual		PxStream&		storeFloat(float f);
	virtual		PxStream&		storeDouble(double d);

    virtual     PxStream&       storeVector2(const vec2& v);
    virtual     PxStream&       storeVector3(const vec3& v);
    virtual     PxStream&       storeVector4(const vec4& v);
    virtual     PxStream&       storeMatrix(const mat44& m);

    virtual     PxStream&       storeString(const std::string& s);
    virtual     PxStream&       storeWString(const std::wstring& s);

	virtual		PxStream&		storeBuffer(const void* buffer, PxU32 size);

private:
    FILE* m_pFile;
    std::string m_FileName;

    //Disable default copy constructor and default assignment operator
    BinaryStream(const BinaryStream&);
    BinaryStream& operator=(const BinaryStream&);
};

} } //end namespace

#endif
