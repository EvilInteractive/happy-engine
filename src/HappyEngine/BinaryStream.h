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

#ifndef _BINARY_STREAM_H_
#define _BINARY_STREAM_H_
#pragma once

#include "common/PxStream.h"
#include "HappyTypes.h"

#include <string>
#include <iostream>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

namespace happyengine {
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

    virtual     math::Vector2   readVector2()                           const;
    virtual     math::Vector3   readVector3()                           const;
    virtual     math::Vector4   readVector4()                           const;
    virtual     math::Matrix    readMatrix()                            const;

    virtual     std::string     readString()                            const;
    virtual     std::wstring    readWString()                           const;

	virtual		void			readBuffer(void* buffer, PxU32 size)	const;

	virtual		PxStream&		storeByte(byte b);
	virtual		PxStream&		storeWord(ushort w);
	virtual		PxStream&		storeDword(uint d);
	virtual		PxStream&		storeFloat(float f);
	virtual		PxStream&		storeDouble(double d);

    virtual     PxStream&       storeVector2(const math::Vector2& v);
    virtual     PxStream&       storeVector3(const math::Vector3& v);
    virtual     PxStream&       storeVector4(const math::Vector4& v);
    virtual     PxStream&       storeMatrix(const math::Matrix& m);

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
