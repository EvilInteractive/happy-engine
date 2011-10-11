//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include <string>

#pragma warning(disable:4127)

namespace he {
namespace err {
namespace details {

#if _DEBUG || TEST
void happyAssert(bool isOk, const std::string& message, const char* file, int line);
void happyAssert(const std::string& message, const char* file, int line);
void happyAssert(bool isOk, const char* file, int line);
void happyAssert(int isOk, const char* file, int line); //for boost
void happyAssert(void* isOk, const char* file, int line); //for boost
#endif

static int s_scope = 0;

} } } //end namespace
////

#if _DEBUG || TEST
#define ASSERT \
if (he::err::details::s_scope < 0) {} \
else \
    struct HappyAssert \
    { \
        static int getLine(int line = __LINE__) \
        { \
            return line; \
        } \
        explicit HappyAssert(bool isOk, const std::string& message = "") \
        { \
            he::err::details::happyAssert(isOk, message, __FILE__, HappyAssert::getLine()); \
        } \
        explicit HappyAssert(const std::string& message) \
        { \
            he::err::details::happyAssert(message, __FILE__, HappyAssert::getLine()); \
        } \
        explicit HappyAssert(void* isOk) \
        { \
            he::err::details::happyAssert(isOk, __FILE__, HappyAssert::getLine()); \
        } \
        explicit HappyAssert(int isOk) \
        { \
            he::err::details::happyAssert(isOk, __FILE__, HappyAssert::getLine()); \
        } \
    } myAssert = HappyAssert
#else
#define ASSERT(...) {}
#endif

#pragma warning(default:4127)

#endif
