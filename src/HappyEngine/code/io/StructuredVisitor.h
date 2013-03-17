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

    virtual void close() = 0;

    virtual bool enterNode(const he::FixedString& key, const char* comment = NULL) = 0;
    virtual void exitNode(const he::FixedString& key) = 0;

    virtual bool visit(const he::FixedString& key, he::String& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, bool& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, int8& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, uint8& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, int16& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, uint16& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, int32& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, uint32& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, int64& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, uint64& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, float& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, double& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, vec2& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, vec3& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, vec4& value, const char* comment = NULL) = 0;
    virtual bool visit(const he::FixedString& key, Guid& value, const char* comment = NULL) = 0;
    
    template<typename EnumType, typename CastType>
    bool visitEnum(const he::FixedString& key, EnumType& enumValue, const char* comment = NULL)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        CastType value;
        if (m_OpenType == eOpenType_Write)
        {
            value = checked_numcast<CastType>(enumValue);
        }
        const bool result(visit(key, value, comment));

        enumValue = checked_numcast<EnumType>(value);

        return result;
    }

protected:
    EOpenType m_OpenType;

private:
    //Disable default copy constructor and default assignment operator
    StructuredVisitor(const StructuredVisitor&);
    StructuredVisitor& operator=(const StructuredVisitor&);
};
    

} } //end namespace

#endif
