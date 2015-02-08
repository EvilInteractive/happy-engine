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
//Created: 2015/01/30

#ifndef HappyNewOverride_h__
#define HappyNewOverride_h__

#include "HappyMemory.h"

#pragma warning( disable : 4290 ) // visual studio does not implement throw specifications
#ifdef HE_DEBUG
void* operator new(std::size_t n DEF_MEM_DEBUG_NFL_PARAMS) throw(std::bad_alloc)
{
    return he::gMemMan->alloc(n PASS_MEM_DEBUG_NFL_PARAMS);
}
void operator delete(void * p DEF_MEM_DEBUG_NFL_PARAMS) throw()
{
    name; file; line;
    he::gMemMan->free(p);
}

#endif

void* operator new(std::size_t n) throw(std::bad_alloc)
{
    return he_malloc("operator new", n);
}
void operator delete(void * p) throw()
{
    he_free(p);
}

#ifdef HE_DEBUG
void *operator new[](std::size_t s DEF_MEM_DEBUG_NFL_PARAMS) throw(std::bad_alloc)
{
    return he::gMemMan->alloc(s PASS_MEM_DEBUG_NFL_PARAMS);
}
void operator delete[](void *p DEF_MEM_DEBUG_NFL_PARAMS) throw()
{
    name; file; line;
    he_free(p);
}
#endif

void *operator new[](std::size_t s) throw(std::bad_alloc)
{
    return he_malloc("operator new[]", s);
}

void operator delete[](void *p) throw()
{
    he_free(p);
}
#pragma warning( default : 4290 ) 

#endif // HappyNewOverride_h__
