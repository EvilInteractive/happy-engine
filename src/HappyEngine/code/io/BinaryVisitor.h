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

#ifndef _HE_BINARY_VISITOR_H_
#define _HE_BINARY_VISITOR_H_
#pragma once

#include "common/PxIO.h"

namespace he {
namespace io {

class HAPPY_ENTRY BinaryVisitor
{
public:
    enum EOpenType
    {
        eOpenType_Read, eOpenType_Write, eOpenType_Closed
    };
    BinaryVisitor(): m_OpenType(eOpenType_Closed) {}
    virtual ~BinaryVisitor() {}

    bool isOpen() const { return m_OpenType != eOpenType_Closed; }

    virtual void skipBytes(const size_t bytes) = 0;
    virtual size_t getProcessedBytes() = 0; // read bytes when reading, written bytes while writing
    
    template<typename T>
    void visit(T& value)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        if (m_OpenType == eOpenType_Read)
        {
            readBuffer(&value, sizeof(T));
        }
        else
        {
            writeBuffer(&value, sizeof(T));
        }
    }


    void visitBlob(const void* buffer, const size_t byteSize)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        HE_ASSERT(m_OpenType == eOpenType_Write, "Stream with const buffer can only be used for writing!");
        writeBuffer(buffer, byteSize);    
    }
    void visitBlob(void* buffer, const size_t byteSize)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        if (m_OpenType == eOpenType_Read)
        {
            readBuffer(buffer, byteSize);
        }
        else
        {
            writeBuffer(buffer, byteSize);
        }
    }

    template<typename EnumType, typename CastType>
    void visitEnum(EnumType& enumValue)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        CastType value;
        if (m_OpenType == eOpenType_Write)
        {
            value = static_cast<CastType>(enumValue);
        }
        visit(value);

        enumValue = static_cast<EnumType>(value);
    }


    virtual void close() = 0;

protected:
    virtual	size_t readBuffer(void* buffer, const size_t byteCount) = 0;
    virtual size_t writeBuffer(const void* buffer, const size_t byteCount) = 0;

    EOpenType m_OpenType;

private:
    //Disable default copy constructor and default assignment operator
    BinaryVisitor(const BinaryVisitor&);
    BinaryVisitor& operator=(const BinaryVisitor&);
};
    
template<>
void BinaryVisitor::visit(he::String& value);

} } //end namespace

#endif
