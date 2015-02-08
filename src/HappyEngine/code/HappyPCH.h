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

#ifndef _HE_HAPPYPCH_H_
#define _HE_HAPPYPCH_H_
#pragma once

#pragma message("-------------------------------------------------")
#pragma message("-- HappyEngine: Compiling precompiled headers. --")
#pragma message("-------------------------------------------------\n")

#ifdef HE_DEBUG
#define _DEBUG 1
#define DEBUG 1
#else
#define NDEBUG 1
#endif

#ifdef HE_WINDOWS
#define __HE_FUNCTION__ __FUNCTION__
#else
#define __HE_FUNCTION__ __PRETTY_FUNCTION__
#endif
#ifdef HE_DEBUG
#define FILE_AND_LINE __FILE__, __LINE__
#define COMMA_FILE_AND_LINE , __FILE__, __LINE__
#else
#define FILE_AND_LINE
#define COMMA_FILE_AND_LINE
#endif

#ifdef HE_WINDOWS
    #ifdef HappyEngine_EXPORTS
        #define HAPPY_ENTRY __declspec(dllexport)
        #define HAPPY_TEMPLATE_ENTRY
    #else
        #define HAPPY_ENTRY __declspec(dllimport)
        #define HAPPY_TEMPLATE_ENTRY extern
    #endif
#else
    #define HAPPY_ENTRY
    #define APIENTRY
#endif

#ifdef _MSC_VER
#define HE_FORCEINLINE __forceinline
#else
#define HE_FORCEINLINE __inline__ __attribute__((always_inline))
#endif

#ifdef _MSC_VER
#define HE_THREAD_LOCAL __declspec(thread)
#else
#define HE_THREAD_LOCAL thread_local
#endif

#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, val, ...) val
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define CONCAT(a, b) CONCAT2(a, b)
#define CONCAT2(a, b) a##b
#define STR(a) #a
#define EVAL(a) a

/*
'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
To minimize the possibility of data corruption when exporting a class with __declspec(dllexport), ensure that:
 - All your static data is access through functions that are exported from the DLL.
 - No inlined methods of your class can modify static data.
 - No inlined methods of your class use CRT functions or other library functions use static data (see Potential Errors Passing CRT Objects Across DLL Boundaries for more information).
 - No methods of your class (regardless of inlining) can use types where the instantiation in the EXE and DLL have static data differences.
--> Make sure the compile options are the same!
*/
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 ) // non dll-interface class 'X' used as base for dll-interface class

#include <atomic>
#ifdef HE_WINDOWS
#include <intsafe.h>
#endif
#include <limits.h>
#include <functional>

#pragma message("-- 30% Done --")
//#include <boost/any.hpp>
#pragma message("-- 40% Done --")
//#include <boost/date_time.hpp>

#if !(defined(HE_WINDOWS) || defined(HE_LINUX) || defined(HE_MAC))
#error Unsupported OS!
#endif

#undef MessageBox

#define glewGetContext() (&he::gfx::GL::s_CurrentContext->getGlewContext())


// Happy Code

#if defined(_MSC_VER) && _MSC_VER < 1700
#define ENUM(name, size) enum name
#else
#define ENUM(name, size) enum name : size
#endif

#include "HappyTargetDefines.h"
#include "FixedString.h"
#include "HappyTypes.h"
#include "HappyMemory.h"
#include "HappyNew.h"
#include "HeString.h"
#include "HeFixedStrings.h"
#pragma message("-- 50% Done --")

#include "HappyGuid.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat44.h"
#include "mat33.h"
#include "Color.h"
#include "Rect.h"

#include "RTTI.h"

#include "thread/Thread.h"
#include "thread/Mutex.h"

#include "Logger.h"
#include "HappyInfo.h"
#include "HeAssert.h"

#include "HappyHelpers.h"
#include "HappyFunctions.h"
#include "MathConstants.h"
#include "MathFunctions.h"
#pragma message("-- 60% Done --")

#include <GL/glew.h>
#include "OpenGL.h"
#include "GLContext.h"
#pragma message("-- 70% Done --")

#include "List.h"
#include "Map.h"
#include "Queue.h"
#include "thread/ThreadTicket.h"
#include "FixedSizeList.h"
#pragma message("-- 80% Done --")

#include "Path.h"
#include "HappyEngine.h"
#include "ObjectFactory.h"
#pragma message("-- 90% Done --")

#include "Profiler.h"

#include "event.h"

#include "Keys.h"
#include "MouseButtons.h"

#include "Console.h"
#pragma message("-- 100% Done --")


#undef near
#undef far

#endif
