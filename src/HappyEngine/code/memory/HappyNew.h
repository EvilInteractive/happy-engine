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
//Created: 23/08/2011

#ifndef _HE_HAPPY_NEW_H_
#define _HE_HAPPY_NEW_H_
#pragma once

#include "MemoryManager.h"

#pragma warning( disable : 4290 ) // visual studio does not implement throw specifications
void* operator new(std::size_t n DEF_MEM_DEBUG_NFL_PARAMS) throw(std::bad_alloc);
void operator delete(void * p DEF_MEM_DEBUG_NFL_PARAMS) throw();
void* operator new[](std::size_t n DEF_MEM_DEBUG_NFL_PARAMS) throw(std::bad_alloc);
void operator delete[](void * p DEF_MEM_DEBUG_NFL_PARAMS) throw();
#pragma warning( default : 4290 )

#ifdef HE_MEMORY_DEBUG
#define HENewArray(cl, count) new(typeid(cl).name(), __FILE__, __LINE__) cl[count]
#define HENewArrayFL(cl, count, file, line) new(typeid(cl).name(), file, line) cl[count]
#define HENew(cl) new(typeid(cl).name(), __FILE__, __LINE__) cl
#define HENewFL(cl, file, line) new cl(typeid(cl).name(), file, line)
#define HEDelete(mem) delete mem
#define HEDeleteArray(mem) delete[] mem
#else
#define HENewArray(cl, count) new cl[count]
#define HENewArrayFL(cl, count, ...) new cl[count]
#define HENew(cl) new cl
#define HENewFL(cl, ...) new cl
#define HEDelete(mem) delete mem
#define HEDeleteArray(mem) delete[] mem
#endif

#define PNEW(mem) new(mem)

#endif