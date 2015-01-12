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
#ifdef HE_DEBUG
    #ifdef _MSC_VER
        #define he_checkmem() _ASSERT_EXPR(_CrtCheckMemory() == TRUE, _CRT_WIDE("Heap check failed! Memcorruption detected"))
    #else
        #define he_checkmem()
    #endif
#else
    #define he_checkmem()
#endif

//////////////////////////////////////////////////////////////////////////
///    malloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_malloc(size) gMemMan->alloc(size, __FILE__, __LINE__)
#else
    #define he_malloc(size) gMemMan->alloc(size)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned malloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_aligned_malloc(size, align) gMemMan->allocAligned(size, align, __FILE__, __LINE__)
#else
    #define he_aligned_malloc(size, align) gMemMan->allocAligned(size, align)
#endif

//////////////////////////////////////////////////////////////////////////
///    free
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_free(mem) gMemMan->free(mem, __FILE__, __LINE__)
#else
    #define he_free(mem) gMemMan->free(mem)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned free
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_aligned_free(mem) gMemMan->freeAligned(mem, __FILE__, __LINE__)
#else
    #define he_aligned_free(mem) gMemMan->freeAligned(mem)
#endif
//////////////////////////////////////////////////////////////////////////
///    realloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_realloc(mem, newsize) gMemMan->realloc(mem, newsize, __FILE__, __LINE__)
#else
    #define he_realloc(mem, newsize) gMemMan->realloc(mem, newsize)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned realloc
//////////////////////////////////////////////////////////////////////////
#ifdef HE_DEBUG
    #define he_aligned_realloc(mem, newsize, alignment) gMemMan->reallocAligned(mem, newsize, alignment, __FILE__, __LINE__)
#else
    #define he_aligned_realloc(mem, newsize, alignment) gMemMan->reallocAligned(mem, newsize, alignment)
#endif

//////////////////////////////////////////////////////////////////////////
///    memset
//////////////////////////////////////////////////////////////////////////
#define he_memset(mem, value, size) memset(mem, value, size)

//////////////////////////////////////////////////////////////////////////
///    memcpy
//////////////////////////////////////////////////////////////////////////
#define he_memcpy(dest, src, size) memcpy(dest, src, size)

//////////////////////////////////////////////////////////////////////////
///    memmove
//////////////////////////////////////////////////////////////////////////
#define he_memmove(dest, src, size) memmove(dest, src, size)

#endif