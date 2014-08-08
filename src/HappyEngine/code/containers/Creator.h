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
class PrimitiveObjectAllocator
{
public:
    static inline T* allocate(const size_t amount) 
    { 
        if (amount != 0)
        {
            T* const mem(static_cast<T*>(he_malloc(amount * sizeof(T)))); 
            he_memset(mem, 0, amount * sizeof(T)); 
            return mem;
        }
        else
        {
            return nullptr;
        }
    }
    static inline void deallocate(T* mem) 
    { 
        he_free(mem); 
    }

    static inline T* reallocate(T* const old, const size_t oldAmount, const size_t newAmount)
    {
        T* const newMem(static_cast<T*>(he_realloc(old, newAmount * sizeof(T))));
        if (oldAmount < newAmount)
            he_memset(newMem + oldAmount, 0, sizeof(T) * (newAmount - oldAmount));
        return newMem;
    }
};

template<typename T>
class ObjectAllocator
{
public:
    static inline T* allocate(const size_t amount) 
    { 
        if (amount != 0)
        {
            T* const mem(NEW T[amount]);
            return mem;
        }
        else
        {
            return nullptr;
        }
    }
    static inline void deallocate(T* mem) 
    { 
        delete[] mem;
    }

    static inline T* reallocate(T* const old, const size_t oldAmount, const size_t newAmount)
    {
        T* const newMem(allocate(newAmount));
        size_t copyAmount(std::min(newAmount, oldAmount));
        for (size_t i(0); i < copyAmount; ++i)
        {
            newMem[i] = std::move(old[i]);
        }
        deallocate(old);
        return newMem;
    }
};

}

#endif
