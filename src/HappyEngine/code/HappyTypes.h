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

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef RakNet::BitStream NetworkStream;
typedef RakNet::RakNetGUID NetworkID;
#define UNASSIGNED_NETWORKID RakNet::UNASSIGNED_RAKNET_GUID

struct NetworkObjectID
{
    uint16 id;

    NetworkObjectID(): id(0xffff) {}
    NetworkObjectID(uint16 _id): id(_id) {}

    bool operator==(const NetworkObjectID& other) const
    {
        return id == other.id;
    }
    bool operator!=(const NetworkObjectID& other) const
    {
        return id != other.id;
    }

    const static NetworkObjectID unassigned;
};

#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

#define OBJECTHANDLE_MAX 0xffff
struct ObjectHandle
{
    typedef ushort Type;
    Type index;
    Type salt;

    ObjectHandle(): index(0xffff), salt(0xffff) {}

    bool operator==(const ObjectHandle& other) const
    {
        return index == other.index && salt == other.salt;
    }
    bool operator!=(const ObjectHandle& other) const
    {
        return index != other.index || salt != other.salt;
    }

    const static ObjectHandle unassigned;
};

} //end namespace


#endif
