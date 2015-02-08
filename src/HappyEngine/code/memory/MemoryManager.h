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

#ifndef MemoryManager_h__
#define MemoryManager_h__

namespace he {

#ifdef HE_DEBUG
#define DEF_MEM_DEBUG_NFL_PARAMS , const char* const name, const char* const file, const int line
#define DEF_MEM_DEBUG_FL_PARAMS , const char* const file, const int line
#define PASS_MEM_DEBUG_NFL_PARAMS , name, file, line
#define PASS_MEM_DEBUG_FL_PARAMS , file, line
#define PASS_MEM_DEBUG_F_PARAM , file
#define PASS_MEM_DEBUG_L_PARAM , line
#define GET_MEM_DEBUG_L_PARAM , __LINE__
#define GET_MEM_DEBUG_F_PARAM , __FILE__
#define GET_MEM_DEBUG_FL_PARAM , __FILE__, __LINE__
#define GET_MEM_DEBUG_NAME_PARAM(x) , typeid(x).name()
#define MEM_DEBUG_PARAM(x) , x
#else
#define DEF_MEM_DEBUG_NFL_PARAMS
#define DEF_MEM_DEBUG_FL_PARAMS
#define PASS_MEM_DEBUG_NFL_PARAMS
#define PASS_MEM_DEBUG_FL_PARAMS
#define PASS_MEM_DEBUG_F_PARAM 
#define PASS_MEM_DEBUG_L_PARAM 
#define GET_MEM_DEBUG_L_PARAM
#define GET_MEM_DEBUG_F_PARAM
#define GET_MEM_DEBUG_FL_PARAM
#define GET_MEM_DEBUG_NAME_PARAM(x)
#define MEM_DEBUG_PARAM(x)
#endif

class MemoryManager;
extern HAPPY_ENTRY MemoryManager* gMemMan;

class HAPPY_ENTRY MemoryManager
{
public:
    MemoryManager();
    ~MemoryManager();

    void* alloc(const size_t size DEF_MEM_DEBUG_NFL_PARAMS);
    void* allocAligned(const size_t size, const size_t alignment DEF_MEM_DEBUG_NFL_PARAMS);
    void* realloc(void* oldmem, const size_t newSize DEF_MEM_DEBUG_NFL_PARAMS);
    void* reallocAligned(void* oldmem, const size_t newSize, const size_t alignment DEF_MEM_DEBUG_NFL_PARAMS);
    void free(void* mem);
    void freeAligned(void* mem);
    size_t memsize(void* mem) const;

    void registerAlloc(void* mem DEF_MEM_DEBUG_NFL_PARAMS);
    void unregisterAlloc(void* mem);

    void checkMem();

    uint64 getTrackedMemory() const;

private:
    void* m_Pool;

    struct MemTracker;
    MemTracker* m_MemTracker;
    
    MemoryManager(const MemoryManager&);
    MemoryManager& operator=(const MemoryManager&);
};

}

#endif // MemoryManager_h__
