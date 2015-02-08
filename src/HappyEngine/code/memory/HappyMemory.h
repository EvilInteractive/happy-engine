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

#ifndef _HE_HAPPYMEMORY_H_
#define _HE_HAPPYMEMORY_H_
#pragma once

#include "MemoryManager.h"

#ifdef _MSC_VER
#define alignas(x) __declspec(align(x))
#endif

//////////////////////////////////////////////////////////////////////////
///    check mem
//////////////////////////////////////////////////////////////////////////
#ifdef HE_MEMORY_DEBUG
    #ifdef _MSC_VER
        #define he_checkmem()  \
            { \
                _ASSERT_EXPR(_CrtCheckMemory() == TRUE, _CRT_WIDE("Heap check failed! Memcorruption detected")); \
                if (he::gMemMan) he::gMemMan->checkMem(); \
            }
    #else
        #define he_checkmem()  \
            { \
                if (he::gMemMan) he::gMemMan->checkMem(); \
            }
    #endif
#else
    #define he_checkmem()
#endif

//////////////////////////////////////////////////////////////////////////
///    memsize
//////////////////////////////////////////////////////////////////////////
#define he_memsize(mem) he::gMemMan->memsize(mem)

//////////////////////////////////////////////////////////////////////////
///    aligned malloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_MEMORY_DEBUG
    #define he_malloc(name, size) he::gMemMan->alloc(size, name, __FILE__, __LINE__)
#else
    #define he_malloc(name, size) he::gMemMan->alloc(size)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned malloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_MEMORY_DEBUG
    #define he_aligned_malloc(name, size, align) he::gMemMan->allocAligned(size, align, name, __FILE__, __LINE__)
#else
    #define he_aligned_malloc(name, size, align) he::gMemMan->allocAligned(size, align)
#endif

//////////////////////////////////////////////////////////////////////////
///    free
//////////////////////////////////////////////////////////////////////////
#define he_free(mem) he::gMemMan->free(mem)

//////////////////////////////////////////////////////////////////////////
///    aligned free
//////////////////////////////////////////////////////////////////////////
#define he_aligned_free(mem) he::gMemMan->freeAligned(mem)

//////////////////////////////////////////////////////////////////////////
///    realloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_MEMORY_DEBUG
    #define he_realloc(name, mem, newsize) he::gMemMan->realloc(mem, newsize, name, __FILE__, __LINE__)
#else
    #define he_realloc(name, mem, newsize) he::gMemMan->realloc(mem, newsize)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned realloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_MEMORY_DEBUG
    #define he_aligned_realloc(name, mem, newsize, alignment) he::gMemMan->reallocAligned(mem, newsize, alignment, name, __FILE__, __LINE__)
#else
    #define he_aligned_realloc(name, mem, newsize, alignment) he::gMemMan->reallocAligned(mem, newsize, alignment)
#endif

//////////////////////////////////////////////////////////////////////////
///    memset
//////////////////////////////////////////////////////////////////////////
#define he_memset(mem, value, size) memset(mem, value, size)

//////////////////////////////////////////////////////////////////////////
///    memcpy
//////////////////////////////////////////////////////////////////////////
#define he_memcpy(dest, src, size) ::memcpy(dest, src, size)

//////////////////////////////////////////////////////////////////////////
///    memmove
//////////////////////////////////////////////////////////////////////////
#define he_memmove(dest, src, size) ::memmove(dest, src, size)

#endif