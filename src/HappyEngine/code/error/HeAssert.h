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
//
//Author:  Bastian Damman
//Created: 13/06/2011
//Updated assert: 04/09/2011

#ifndef _HE_ASSERT_H_
#define _HE_ASSERT_H_
#pragma once

#pragma warning(disable:4127)

namespace he {
namespace err {
namespace details {

#if _DEBUG || TEST
bool happyAssert(bool isOk, const char* file, const char* func, int line, const char* message, ...);
#endif

static int s_scope = 0;

} } } //end namespace
////

//#if _DEBUG || TEST
//#define HE_ASSERT \
//if (he::err::details::s_scope < 0) {} \
//else \
//    struct HappyAssert \
//    { \
//        static int getLine(int line = __LINE__) \
//        { \
//            return line; \
//        } \
//        static const char* getFunc(const char* func = __HE_FUNCTION__) \
//        { \
//            return func; \
//        } \
//        explicit HappyAssert(bool isOk, const char* message = "", ...) \
//        { \
//            va_list arg_list;\
//            va_start(arg_list, message);\
//            he::err::details::happyAssert(isOk, __FILE__, HappyAssert::getFunc(), HappyAssert::getLine(), message, arg_list); \
//            va_end(arg_list);\
//        } \
//        explicit HappyAssert(void* isOk) \
//        { \
//            he::err::details::happyAssert(isOk, __FILE__, HappyAssert::getFunc(), HappyAssert::getLine()); \
//        } \
//        explicit HappyAssert(int isOk) \
//        { \
//            he::err::details::happyAssert(isOk, __FILE__, HappyAssert::getFunc(), HappyAssert::getLine()); \
//        } \
//    } myAssert = HappyAssert
//#else
//#define HE_ASSERT(...) {}
//#endif


#if _DEBUG || TEST
#define HE_ASSERT(isOk, message, ...) he::err::details::happyAssert(isOk, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define HE_IF_ASSERT(isOk, message, ...) if (he::err::details::happyAssert(isOk, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__) == true)
#else
#define HE_ASSERT(isOk, message, ...) {}
#define HE_IF_ASSERT(isOk, message, ...) {}            // faster version
//#define HE_IF_ASSERT(isOk, message, args) if (isOk)  // safe version
#endif

#pragma warning(default:4127)

#endif
