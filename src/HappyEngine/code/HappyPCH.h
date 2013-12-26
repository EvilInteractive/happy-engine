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

#ifndef _HE_HAPPYPCH_H_
#define _HE_HAPPYPCH_H_
#pragma once

#pragma message("-------------------------------------------------")
#pragma message("-- HappyEngine: Compiling precompiled headers. --")
#pragma message("-------------------------------------------------\n")

#ifndef NDEBUG
#define _DEBUG 1
#endif

#ifdef HE_WINDOWS
#define __HE_FUNCTION__ __FUNCTION__
#else
#define __HE_FUNCTION__ __PRETTY_FUNCTION__
#endif
#ifdef _DEBUG
#define FILE_AND_LINE __FILE__, __LINE__
#else
#define FILE_AND_LINE
#endif

#ifdef HE_WINDOWS
    #ifdef HappyEngine_EXPORTS
        #define HAPPY_ENTRY __declspec(dllexport)
    #else
        #define HAPPY_ENTRY __declspec(dllimport)
    #endif
#else
    #define HAPPY_ENTRY
    #define APIENTRY
#endif

#define HE_FORCEINLINE __forceinline


#define CONCAT(a, b) a##b
#define STR(a) #a

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

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <queue>
#include <set>
#include <atomic>

#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/date_time.hpp>
#include <boost/timer.hpp>

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
#include "HeString.h"
#include "FixedString.h"
#include "HeFixedStrings.h"

#include "HappyTypes.h"
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

#ifdef _DEBUG
#pragma warning(disable:4389) // '==' signed/unsigned mismatch
template<typename To, typename From>
inline To checked_numcast(const From value)
{
    const To result(static_cast<To>(value));
    HE_ASSERT(result == value, "Numcast fail!");
    return result;
}
#pragma warning(default:4389)
template<typename To, typename From>
inline To checked_cast(From const value)
{
    To const result(dynamic_cast<To>(value));
    HE_ASSERT(value == nullptr || nullptr != result, "checked cast failed!");
    return result;
}
#else
template<typename To, typename From>
inline To checked_numcast(const From value)
{
    return static_cast<To>(value);
}
template<typename To, typename From>
inline To checked_cast(From value)
{
    return static_cast<To>(value);
}
#endif

#include "HappyMemory.h"
#include "HappyNew.h"
#include "MathConstants.h"
#include "MathFunctions.h"

#include <GL/glew.h>
#include "OpenGL.h"
#include "GLContext.h"

#include "List.h"
#include "FixedSizeList.h"

#include "ObjectFactory.h"

#include "Profiler.h"

#include "event.h"
#include "Path.h"

#include "Keys.h"
#include "MouseButtons.h"

#include "HappyEngine.h"
#include "Console.h"


#undef near
#undef far

#endif
