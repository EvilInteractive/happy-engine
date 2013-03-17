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

namespace he {
namespace io {

class HAPPY_ENTRY StructuredVisitor
{
public:
    enum EOpenType
    {
        eOpenType_Read, eOpenType_Write, eOpenType_Closed
    };
    StructuredVisitor(): m_OpenType(eOpenType_Closed) {}
    virtual ~StructuredVisitor() {}

    bool isOpen() const { return m_OpenType != eOpenType_Closed; }
    
    template<typename T>
    void visit(const he::FixedString& key, T& value)
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
    
    template<typename EnumType, typename CastType>
    void visitEnum(const he::FixedString& key, EnumType& enumValue)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        CastType value;
        if (m_OpenType == eOpenType_Write)
        {
            value = checked_numcast<CastType>(enumValue);
        }
        visit(value);

        enumValue = checked_numcast<EnumType>(value);
    }


    virtual void close() = 0;

protected:
    virtual	size_t readBuffer(void* buffer, const size_t byteCount) = 0;
    virtual size_t writeBuffer(const void* buffer, const size_t byteCount) = 0;

    EOpenType m_OpenType;

private:
    //Disable default copy constructor and default assignment operator
    StructuredVisitor(const StructuredVisitor&);
    StructuredVisitor& operator=(const StructuredVisitor&);
};
    
template<>
void StructuredVisitor::visit(he::String& value);

} } //end namespace

#endif
