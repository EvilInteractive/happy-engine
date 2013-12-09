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
//Created: 05/08/2011

#ifndef _HE_HAPPY_TYPES_H_
#define _HE_HAPPY_TYPES_H_
#pragma once

#include <stdint.h>
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
    eLoadResult_Unknown,
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

#define OBJECTHANDLE_MAX 0xffff
struct ObjectHandle
{
    typedef uint16 ObjectType;
    typedef uint32 SaltType;
    typedef uint16 IndexType;

    static const int MAX_INDEX = UINT16_MAX;
    static const int MAX_SALT = UINT32_MAX;
    static const int MAX_TYPE = UINT16_MAX;

    static const int UNASSIGNED_TYPE = 0;

    IndexType   index;
    SaltType    salt;
    ObjectType  type;

    ObjectHandle(): index(0), salt(0), type(UNASSIGNED_TYPE)  {}
    ~ObjectHandle() {}

    bool operator==(const ObjectHandle& other) const
    {
        return type == other.type && index == other.index && salt == other.salt;
    }
    bool operator!=(const ObjectHandle& other) const
    {
        return type != other.type || index != other.index || salt != other.salt;
    }

    const static ObjectHandle unassigned;
};

struct HAPPY_ENTRY Guid
{
    Guid();
    ~Guid() {}
    explicit Guid(const char* const guid);
    Guid(const Guid& other);
    Guid& operator=(const Guid& other); // 921E39A0-F8A8-4131-BB74-31968CF5A9E6

    he::String toString() const;

    static Guid generateGuid();

    bool operator==(const Guid& other) const;
    bool operator!=(const Guid& other) const;

    uint32 m_Data1;
    // -
    uint16 m_Data2;
    // -
    uint16 m_Data3;
    // -
    uint16 m_Data4;
    // -
    uint16 m_Data5;
    uint32 m_Data6;
};

} //end namespace


#endif
