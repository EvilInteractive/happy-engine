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
#define DEF_FILE_AND_LINE , const char* const file, const int line
#else
#define DEF_FILE_AND_LINE
#endif

class MemoryManager;
extern HAPPY_ENTRY MemoryManager* gMemMan;

HAPPY_ENTRY void* globalAllocate(const size_t size DEF_FILE_AND_LINE);
HAPPY_ENTRY void globalFree(void* const mem DEF_FILE_AND_LINE);
HAPPY_ENTRY void* globalRealloc(void* const oldmem, const size_t size DEF_FILE_AND_LINE);

class HAPPY_ENTRY MemoryManager
{
public:
    MemoryManager();
    ~MemoryManager();

    void* alloc(const size_t size DEF_FILE_AND_LINE);
    void* allocAligned(const size_t size, const size_t alignment DEF_FILE_AND_LINE);
    void* realloc(void* oldmem, const size_t newSize DEF_FILE_AND_LINE);
    void* reallocAligned(void* oldmem, const size_t newSize, const size_t alignment DEF_FILE_AND_LINE);
    void free(void* mem DEF_FILE_AND_LINE);
    void freeAligned(void* mem DEF_FILE_AND_LINE);

private:
    MemoryManager(const MemoryManager&);
    MemoryManager& operator=(const MemoryManager&);
};

#undef DEF_FILE_AND_LINE

}

#endif // MemoryManager_h__
