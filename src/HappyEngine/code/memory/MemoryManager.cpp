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
//
//Author:  Bastian Damman
//Created: 2015/01/12
#include "HappyPCH.h"
#include "MemoryManager.h"

#include "HappyNewOverride.h"
#include "thread/Mutex.h"

// Compile the c code here
#pragma warning(disable:4702) // unused local func is removed
#include <nedmalloc/nedmalloc.h>
#include <nedmalloc/nedmalloc.c>
#pragma warning(default:4702) // unused local func is removed

#include <unordered_map>

#ifdef HE_MEMORY_DEBUG
#define MEM_HIDE_FILE_LINE file; line;
#else
#define MEM_HIDE_FILE_LINE
#endif

namespace he {

static he::uint64 s_BreakAlloc = 0;

#define POOL static_cast<nedalloc::nedpool*>(m_Pool)

MemoryManager* gMemMan(nullptr);

#ifdef HE_MEMORY_DEBUG
template <typename T>
class MemTrackerAllocater: public std::allocator<T>
{
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    template<typename _Tp1>
    struct rebind
    {
        typedef MemTrackerAllocater<_Tp1> other;
    };

    pointer allocate(size_type n, const void * /*hint*/ = 0)
    {
        return new(::malloc(sizeof(T) * n)) T;
    }

    void deallocate(pointer p, size_type /*n*/)
    {
        ::free(p);
    }

    MemTrackerAllocater() throw(): std::allocator<T>() { }
    MemTrackerAllocater(MemTrackerAllocater &a) throw(): std::allocator<T>(a) { }
    template <class U>                    
    MemTrackerAllocater(const MemTrackerAllocater<U> &a) throw(): std::allocator<T>(a) { }
    ~MemTrackerAllocater() throw() { }
};

struct MemoryManager::MemTracker
{
    struct AllocInfo
    {
        AllocInfo() : m_File(nullptr), m_Line(0) {}
        AllocInfo(const char* name, const char* file, const int line) : m_Name(name), m_File(file), m_Line(line), m_AllocID(++s_AllocID) 
        {
            if (s_BreakAlloc == m_AllocID)
            {
                __debugbreak();
            }
        }
        const char* m_Name;
        const char* m_File;
        int m_Line;
        uint64 m_AllocID;
    };
    MemTracker() : m_TrackedMemory(0) {}
    ~MemTracker() {}

    static uint64 s_AllocID;

    uint64 m_TrackedMemory;
    he::Mutex m_MemTrackerMutex;
    typedef std::unordered_map<void*, AllocInfo, std::hash<void*>, std::equal_to<void*>, he::MemTrackerAllocater<std::pair<void*, AllocInfo>>> TMemMap;
    TMemMap m_MemMap;
};
uint64 MemoryManager::MemTracker::s_AllocID(0);
#endif
  
MemoryManager::MemoryManager()
{
    m_Pool = nedalloc::nedcreatepool(0, 0);
#ifdef HE_MEMORY_DEBUG
    m_MemTracker = new(::malloc(sizeof(MemTracker)))MemTracker();
#endif
}

MemoryManager::~MemoryManager()
{
#ifdef HE_MEMORY_DEBUG
    char buff[4 * 1024];
    OutputDebugString("Checking leaks...");
    std::cout << "Checking leaks...";
    std::for_each(m_MemTracker->m_MemMap.cbegin(), m_MemTracker->m_MemMap.cend(), [this, &buff](const std::pair<void*, MemTracker::AllocInfo>& pair)
    {
        hesnprintf_s(buff, 4 * 1024 - 1, "\n%s(%d): Leak detected (#%llu) @0x%x (%d bytes) named %s", pair.second.m_File, pair.second.m_Line, pair.second.m_AllocID, pair.first, memsize(pair.first), pair.second.m_Name);
        std::cout << buff;
        OutputDebugString(buff);
    });
    OutputDebugString("   Done\n");
    std::cout << "   Done\n";
    m_MemTracker->~MemTracker();
    ::free(m_MemTracker);
#endif

    nedalloc::neddestroypool(POOL);
}

void* MemoryManager::alloc( const size_t size DEF_MEM_DEBUG_NFL_PARAMS )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    void* mem(nedalloc::nedpmalloc(POOL, size));
    registerAlloc(mem PASS_MEM_DEBUG_NFL_PARAMS);
    return mem;
}

void* MemoryManager::allocAligned( const size_t size, const size_t alignment DEF_MEM_DEBUG_NFL_PARAMS )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    void* mem(nedalloc::nedpmalloc2(POOL, size, alignment));
    registerAlloc(mem PASS_MEM_DEBUG_NFL_PARAMS);
    return mem;
}

void* MemoryManager::realloc( void* oldmem, const size_t newSize DEF_MEM_DEBUG_NFL_PARAMS )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    unregisterAlloc(oldmem);
    void* mem(nedalloc::nedprealloc(POOL, oldmem, newSize));
    registerAlloc(mem PASS_MEM_DEBUG_NFL_PARAMS);
    return mem;
}

void* MemoryManager::reallocAligned( void* oldmem, const size_t newSize, const size_t alignment DEF_MEM_DEBUG_NFL_PARAMS )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    unregisterAlloc(oldmem);
    void* mem(nedalloc::nedprealloc2(POOL, oldmem, newSize, alignment));
    registerAlloc(mem PASS_MEM_DEBUG_NFL_PARAMS);
    return mem;
}

void MemoryManager::free( void* mem )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    if (mem)
    {
        unregisterAlloc(mem);
        nedalloc::nedpfree(POOL, mem);
    }
}

void MemoryManager::freeAligned( void* mem )
{
    HE_ASSERT(m_Pool, "No default memory pool!");
    if (mem)
    {
        unregisterAlloc(mem);
        nedalloc::nedpfree(POOL, mem);
    }
}

void MemoryManager::registerAlloc( void* mem DEF_MEM_DEBUG_NFL_PARAMS )
{
    mem;
#ifdef HE_MEMORY_DEBUG
    m_MemTracker->m_MemTrackerMutex.lock(FILE_AND_LINE);
    m_MemTracker->m_TrackedMemory += memsize(mem);
    m_MemTracker->m_MemMap[mem] = MemTracker::AllocInfo(name, file, line);
    m_MemTracker->m_MemTrackerMutex.unlock();
#endif
}

void MemoryManager::unregisterAlloc( void* mem )
{
    mem;
#ifdef HE_MEMORY_DEBUG
    m_MemTracker->m_MemTrackerMutex.lock(FILE_AND_LINE);
    m_MemTracker->m_TrackedMemory -= memsize(mem);
    m_MemTracker->m_MemMap.erase(mem);
    m_MemTracker->m_MemTrackerMutex.unlock();
#endif
}

size_t MemoryManager::memsize( void* mem ) const
{
    int isforeign(0);
    size_t size(nedalloc::nedblksize(&isforeign, mem));
    assert(!isforeign);
    return size;
}

he::uint64 MemoryManager::getTrackedMemory() const
{
#ifdef HE_MEMORY_DEBUG
    return m_MemTracker->m_TrackedMemory;
#else
    return 0;
#endif
}

void MemoryManager::checkMem()
{
#ifdef HE_MEMORY_DEBUG
    nedalloc::nedpmallinfo(POOL);
#endif
}

}
#undef MEM_HIDE_FILE_LINE