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

template <typename T>
class StlAllocater: public std::allocator<T>
{
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    template<typename _Tp1>
    struct rebind
    {
        typedef StlAllocater<_Tp1> other;
    };

    pointer allocate(size_type n, const void * /*hint*/ = 0)
    {
        return ObjectAllocator<T>::allocate(n GET_MEM_DEBUG_FL_PARAM);
    }

    void deallocate(pointer p, size_type /*n*/)
    {
        ObjectAllocator<T>::deallocate(p);
    }

    StlAllocater() throw(): std::allocator<T>() { }
    StlAllocater(const StlAllocater &a) throw(): std::allocator<T>(a) { }
    template <class U>                    
    StlAllocater(const StlAllocater<U> &a) throw(): std::allocator<T>(a) { }
    ~StlAllocater() throw() { }
};

template<typename T>
class PrimitiveObjectAllocator
{
public:
    static inline T* allocate(const size_t amount DEF_MEM_DEBUG_FL_PARAMS) 
    { 
        if (amount != 0)
        {
            T* const mem(static_cast<T*>(he::gMemMan->alloc(amount * sizeof(T) GET_MEM_DEBUG_NAME_PARAM(T) PASS_MEM_DEBUG_FL_PARAMS))); 
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
        he::gMemMan->free(mem); 
    }

    static inline T* reallocate(T* const old, const size_t oldAmount, const size_t newAmount DEF_MEM_DEBUG_FL_PARAMS)
    {
        T* const newMem(static_cast<T*>(he::gMemMan->realloc(old, newAmount * sizeof(T) GET_MEM_DEBUG_NAME_PARAM(T) PASS_MEM_DEBUG_FL_PARAMS)));
        if (oldAmount < newAmount)
            he_memset(newMem + oldAmount, 0, sizeof(T) * (newAmount - oldAmount));
        return newMem;
    }

    static inline void memmove(T* const dest, const T* const src, const size_t count)
    {
        he_memmove(dest, src, sizeof(T) * count);
    }
};

template<typename T>
class ObjectAllocator
{
public:
    static inline T* allocate(const size_t amount DEF_MEM_DEBUG_FL_PARAMS) 
    { 
        if (amount != 0)
        {
#ifdef HE_MEMORY_DEBUG
            T* const mem(HENewArrayFL(T, amount, file, line));
#else
            T* const mem(HENewArrayFL(T, amount));
#endif
            return mem;
        }
        else
        {
            return nullptr;
        }
    }
    static inline void deallocate(T* mem) 
    { 
        HEDeleteArray(mem);
    }

    static inline T* reallocate(T* const old, const size_t oldAmount, const size_t newAmount DEF_MEM_DEBUG_FL_PARAMS)
    {
        T* const newMem(allocate(newAmount PASS_MEM_DEBUG_FL_PARAMS));
        const size_t copyAmount(newAmount < oldAmount? newAmount : oldAmount);
        for (size_t i(0); i < copyAmount; ++i)
        {
            newMem[i] = std::move(old[i]);
        }
        deallocate(old);
        return newMem;
    }

    static inline void memmove(T* dst, T* src, size_t count)
    {
        if (dst <= src || dst >= src + count) 
        {
            /*
            * Non-Overlapping Buffers
            * copy from lower addresses to higher addresses
            */
            while (count--) 
            {
                *dst = std::move(*src);
                ++dst;
                ++src;
            }
        }
        else 
        {
            /*
            * Overlapping Buffers
            * copy from higher addresses to lower addresses
            */
            dst = dst + count - 1;
            src = src + count - 1;

            while (count--) 
            {
                *dst = std::move(*src);
                --dst;
                --src;
            }
        }
    }
};

}

#endif
