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
//Created: 23/02/2013

#ifndef _HE_NETWORK_VISITOR_H_
#define _HE_NETWORK_VISITOR_H_
#pragma once

namespace he {
namespace net {

class NetworkVisitor
{
public:
    enum EOpenType
    {
        eOpenType_Read, eOpenType_Write
    };
    NetworkVisitor(RakNet::BitStream* const stream, const EOpenType openType): m_Stream(stream), m_OpenType(openType) {}
    ~NetworkVisitor() {}
        

    EOpenType getOpenType() const { return m_OpenType; }

    template<typename T>
    bool visit(T& value)
    {
        return m_Stream->Serialize<T>(m_OpenType == eOpenType_Write, value);
    }

    bool visitBlob(const void* const buffer, const size_t byteSize)
    {
        HE_ASSERT(m_OpenType == eOpenType_Write, "Stream with const buffer can only be used for writing!");
        m_Stream->Write(static_cast<const char*>(buffer), checked_numcast<unsigned int>(byteSize));
        return true;
    }
    bool visitBlob(void* buffer, const size_t byteSize)
    {
        m_Stream->Serialize(m_OpenType == eOpenType_Write, static_cast<char*>(buffer), checked_numcast<unsigned int>(byteSize));
    }

    template<typename EnumType, typename CastType>
    bool visitEnum(EnumType& enumValue)
    {
        HE_ASSERT(m_OpenType != eOpenType_Closed, "Stream is closed!");
        CastType value;
        if (m_OpenType == eOpenType_Write)
        {
            value = checked_numcast<CastType>(enumValue);
        }
        const bool result(visit(value));

        if (result == true)
        {
            enumValue = checked_numcast<EnumType>(value);
        }

        return result;
    }

    bool visitNormalizedVector(vec3& normal)
    {
        m_Stream->SerializeNormVector(m_OpenType == eOpenType_Write, normal.x, normal.y, normal.z);
    }

private:
    RakNet::BitStream* m_Stream;

    EOpenType m_OpenType;

    //Disable default copy constructor and default assignment operator
    NetworkVisitor(const NetworkVisitor&);
    NetworkVisitor& operator=(const NetworkVisitor&);
};
    
template<>
bool NetworkVisitor::visit(he::String& value);

} } //end namespace

#endif
