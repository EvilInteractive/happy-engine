//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 05/08/2011

#ifndef _HE_HAPPY_TYPES_H_
#define _HE_HAPPY_TYPES_H_
#pragma once

#include <stdint.h>
#include <intsafe.h>
#include "BitStream.h"

namespace he {
    
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint32 SceneID;

enum IntersectResult
{
    IntersectResult_Inside,
    IntersectResult_Intersecting,
    IntersectResult_Outside
};
    
enum ELoadResult
{
    eLoadResult_Unloaded,
    eLoadResult_Success,
    eLoadResult_Failed
};
    
namespace net {
typedef RakNet::BitStream NetworkStream;
typedef RakNet::RakNetGUID NetworkID;
#define UNASSIGNED_NETWORKID RakNet::UNASSIGNED_RAKNET_GUID

struct NetworkObjectTypeID
{
    uint16 id;

    NetworkObjectTypeID(): id(0xffff) {}
    NetworkObjectTypeID(uint16 _id): id(_id) {}

    bool operator==(const NetworkObjectTypeID& other) const
    {
        return id == other.id;
    }
    bool operator!=(const NetworkObjectTypeID& other) const
    {
        return id != other.id;
    }

    const static NetworkObjectTypeID unassigned;
};

}

#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

struct ObjectHandle
{
    static const int s_UnassignedType = 0;

    static const int s_MaxTypes = 64;
    static const int s_MaxSalts = 1024;
    static const int s_MaxIndices = 65536;

    static const int s_TypeMask = 0x3f;
    static const int s_SaltMask = 0xffc0;
    static const int s_IndexMask = 0xffff0000;

    static const int s_TypeBits = 6;
    static const int s_SaltBits = 10;
    static const int s_IndexBits = 16;

    typedef uint32 ObjectType;
    typedef uint32 SaltType;
    typedef uint32 IndexType;
    
    uint32 m_Handle;
    // 32 bit
    // 6 bit : types
    // 10 bit : salts
    // 16 bit : objects

    ObjectHandle()
    : m_Handle(0) {}
    explicit ObjectHandle(const uint32 handle)
    : m_Handle(handle) {}
    ObjectHandle(const ObjectType type, const SaltType salt, const IndexType index)
        : m_Handle((type & s_TypeMask) + ((salt << s_TypeBits) & s_SaltMask) + ((index << (s_TypeBits + s_SaltBits)) & s_IndexMask) ) {}
    ~ObjectHandle() {}

    bool operator==(const ObjectHandle& other) const
    {
        return m_Handle == other.m_Handle;
    }
    bool operator!=(const ObjectHandle& other) const
    {
        return m_Handle != other.m_Handle;
    }

    HE_FORCEINLINE ObjectType getType() const { return m_Handle & s_TypeMask; }
    HE_FORCEINLINE SaltType getSalt() const { return (m_Handle & s_SaltMask) >> s_TypeBits ; }
    HE_FORCEINLINE IndexType getIndex() const { return (m_Handle & s_IndexMask) >> (s_TypeBits + s_SaltBits); }

    const static ObjectHandle unassigned;
};

template<typename T>
struct NameValuePair
{
    he::FixedString m_Name;
    T m_Value;
};

} //end namespace


#endif
