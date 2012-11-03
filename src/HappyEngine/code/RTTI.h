//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 05/08/2012

#ifndef _HE_RTTI_H_
#define _HE_RTTI_H_
#pragma once

namespace he {

typedef uint32 RTTItype;

#define DECLARE_RTTI(rtti) static he::RTTItype getRTTI() { return rtti; }
class RTTI
{
public:
    enum
    {
        // PhysxUserdata clashes:
        Entity =            1 << 0,

    };


    static bool isA(RTTItype typeA, RTTItype typeB)
    {
        return (typeA & typeB) == typeB;
    }
    static bool isExactA(RTTItype typeA, RTTItype typeB)
    {
        return typeA == typeB;
    }
};

} //end namespace

#endif
