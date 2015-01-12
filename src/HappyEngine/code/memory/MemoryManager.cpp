//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/01/12
#include "HappyPCH.h"
#include "MemoryManager.h"

// Compile the c code here
#define NDEBUG
#pragma warning(disable:4702) // unused local func is removed
#include <nedmalloc/nedmalloc.h>
#include <nedmalloc/nedmalloc.c>
#pragma warning(default:4702) // unused local func is removed
#undef NDEBUG

#ifdef HE_DEBUG
#define DEF_FILE_AND_LINE , const char* const file, const int line
#define IMPL_FILE_AND_LINE , file, line
#define HIDE_FILE_LINE file; line;
#else
#define DEF_FILE_AND_LINE
#define IMPL_FILE_AND_LINE
#define HIDE_FILE_LINE
#endif

namespace he {

MemoryManager* gMemMan(nullptr);

void* globalAllocate( const size_t size DEF_FILE_AND_LINE )
{
    return gMemMan->alloc(size IMPL_FILE_AND_LINE);
}

void globalFree( void* const mem DEF_FILE_AND_LINE )
{
    gMemMan->free(mem IMPL_FILE_AND_LINE);
}

void* globalRealloc( void* const oldmem, const size_t size DEF_FILE_AND_LINE )
{
    return gMemMan->realloc(oldmem, size IMPL_FILE_AND_LINE);
}
    
MemoryManager::MemoryManager()
{

}

MemoryManager::~MemoryManager()
{

}

void* MemoryManager::alloc( const size_t size DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedmalloc(size);
}

void* MemoryManager::allocAligned( const size_t size, const size_t alignment DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedmalloc2(size, alignment);
}

void* MemoryManager::realloc( void* oldmem, const size_t newSize DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedrealloc(oldmem, newSize);
}

void* MemoryManager::reallocAligned( void* oldmem, const size_t newSize, const size_t alignment DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedrealloc2(oldmem, newSize, alignment);
}

void MemoryManager::free( void* mem DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedfree(mem);
}

void MemoryManager::freeAligned( void* mem DEF_FILE_AND_LINE )
{
    HIDE_FILE_LINE;
    return nedalloc::nedfree(mem);
}

}

#undef DEF_FILE_AND_LINE
#undef IMPL_FILE_AND_LINE
