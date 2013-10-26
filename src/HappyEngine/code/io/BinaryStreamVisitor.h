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

#ifndef _HE_BinaryStreamVisitor_H_
#define _HE_BinaryStreamVisitor_H_
#pragma once

#include "BinaryVisitor.h"

namespace he {
namespace io {

class BinaryStreamVisitor : public BinaryVisitor
{
public:
    BinaryStreamVisitor();
    virtual ~BinaryStreamVisitor();

    void openWrite();
    void openRead();
    virtual void close();

    const uint8* getData() const;
    size_t getByteCount() const;

    virtual void skipBytes(const size_t bytes);
    virtual size_t getProcessedBytes();

protected:
    virtual	size_t readBuffer(void* buffer, const size_t byteCount);
    virtual size_t writeBuffer(const void* buffer, const size_t byteCount);

private:
    PrimitiveList<uint8> m_Data;
    size_t m_ReadIndex;

    //Disable default copy constructor and default assignment operator
    BinaryStreamVisitor(const BinaryStreamVisitor&);
    BinaryStreamVisitor& operator=(const BinaryStreamVisitor&);
};

} } //end namespace

#endif
