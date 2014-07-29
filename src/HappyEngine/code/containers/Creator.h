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
//Created: 03/11/2012

#ifndef _HE_CREATOR_H_
#define _HE_CREATOR_H_
#pragma once

namespace he {

template<typename T>
class PrimitiveCreator
{
public:
    static inline void create(T* mem) { he_memset(mem, 0, sizeof(T)); }
    static inline void createArray(T* mem, size_t num) { he_memset(mem, 0, sizeof(T) * num); }
    static inline void destroy(T* /*mem*/) {}
    static inline void destroyArray(T* /*mem*/, size_t /*num*/) {}
};

template<typename T>
class ObjectCreator
{
public:
    static inline void create(T* mem) { PNEW(mem) T; }
    static inline void createArray(T* mem, size_t num)
    {
        for (size_t i(0); i < num; ++i)
            create(mem + i);
    }
    static inline void destroy(T* mem) { (void)mem->~T(); }
    static inline void destroyArray(T* mem, size_t num) 
    {
        for (size_t i(0); i < num; ++i)
            destroy(mem + i);
    }
};

}

#endif
