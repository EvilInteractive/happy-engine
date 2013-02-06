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

#ifndef _HE_HAPPYMEMORY_H_
#define _HE_HAPPYMEMORY_H_
#pragma once


//////////////////////////////////////////////////////////////////////////
///    malloc
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_malloc(size) malloc(size)
    #else
        #define he_malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#else
    #define he_malloc(size) malloc(size)
#endif

//////////////////////////////////////////////////////////////////////////
///    calloc -> inits to 0
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #ifdef GCC
        #define he_calloc(num,size) calloc(num,size)
    #else
        #define he_calloc(num,size) _calloc_dbg(num, size, _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#else
    #define he_calloc(num,size) calloc(num,size)
#endif

//////////////////////////////////////////////////////////////////////////
///    free
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_free(mem) free(mem)
    #else
        #define he_free(mem) _free_dbg(mem, _NORMAL_BLOCK)
    #endif
#else
    #define he_free(mem) free(mem)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned malloc
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_aligned_malloc(size, alignment)  ([](unsigned int s, unsigned int a)  -> void* \
        { \
        void* p; \
        posix_memalign(&p, a, s); \
        return p; \
        })(size, alignment)
    #else
        #define he_aligned_malloc(size, alignment) _aligned_malloc_dbg(size, alignment, __FILE__, __LINE__)
    #endif
#else
    #if GCC || LLVM
        #define he_aligned_malloc(size, alignment)  ([](unsigned int s, unsigned int a) -> void* \
        { \
            void* p; \
            posix_memalign(&p, a, s); \
            return p; \
        })(size, alignment)
    #else
        #define he_aligned_malloc(size, alignment) _aligned_malloc(size, alignment)
    #endif
#endif

#endif

//////////////////////////////////////////////////////////////////////////
///    aligned free
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_aligned_free(mem) free(mem)
    #else
        #define he_aligned_free(mem) _aligned_free_dbg(mem)
    #endif
#else
    #if GCC || LLVM
        #define he_aligned_free(mem) free(mem)
    #else
        #define he_aligned_free(mem) _aligned_free(mem)
    #endif
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

//////////////////////////////////////////////////////////////////////////
///    realloc
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_realloc(mem, newsize) realloc(mem, newsize)
    #else
        #define he_realloc(mem, newsize) _realloc_dbg(mem, newsize, _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#else
    #define he_realloc(mem, newsize) realloc(mem, newsize)
#endif

//////////////////////////////////////////////////////////////////////////
///    aligned realloc
//////////////////////////////////////////////////////////////////////////
#if DEBUG || _DEBUG
    #if GCC || LLVM
        #define he_aligned_realloc(memory, oldsize, newsize, alignment) ([](void* mem, unsigned int oSize, unsigned int nSize, unsigned int al) -> void* \
                { \
                    void* temp = he_aligned_malloc(nSize, al); \
                    he_memcpy(temp, mem, nSize < oSize? nSize : oSize); \
                    he_aligned_free(mem); \
                    return temp; \
                })(memory, oldsize, newsize, alignment)
    #else
        #define he_aligned_realloc(mem, oldsize, newsize, alignment) _aligned_realloc_dbg(mem, newsize, alignment, __FILE__, __LINE__)
    #endif
#else
    #if GCC || LLVM
        #define he_aligned_realloc(memory, oldsize, newsize, alignment) ([](void* mem, unsigned int oSize, unsigned int nSize, unsigned int al) -> void* \
                    { \
                        void* temp = he_aligned_malloc(nSize, al); \
                        he_memcpy(temp, mem, nSize < oSize? nSize : oSize); \
                        he_aligned_free(mem); \
                        return temp; \
                    })(memory, oldsize, newsize, alignment)
    #else
        #define he_aligned_realloc(mem, oldsize, newsize, alignment) _aligned_realloc(mem, newsize, alignment)
    #endif
#endif
