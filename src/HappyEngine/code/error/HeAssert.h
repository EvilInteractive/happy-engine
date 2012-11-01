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

enum AssertType
{
    AssertType_Code,
    AssertType_Art
};

#if _DEBUG || TEST
bool happyAssert(bool isOk, AssertType type, const char* file, const char* func, int line, const char* message, ...);
#endif

static int s_scope = 0;

} } } //end namespace

#if _DEBUG || TEST
#define HE_ASSERT(isOk, message, ...) he::err::details::happyAssert(isOk, he::err::details::AssertType_Code, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define HE_IF_ASSERT(isOk, message, ...) if (he::err::details::happyAssert(isOk, he::err::details::AssertType_Code, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__) == true)
#define HE_ART_ASSERT(isOk, message, ...) he::err::details::happyAssert(isOk, he::err::details::AssertType_Art, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define HE_IF_ART_ASSERT(isOk, message, ...) if (he::err::details::happyAssert(isOk, he::err::details::AssertType_Art, __FILE__, __HE_FUNCTION__, __LINE__, message, ##__VA_ARGS__) == true)
#else
#define HE_ASSERT(...) {}
#define HE_IF_ASSERT(...) {}
#define HE_ART_ASSERT(...) {}
#define HE_IF_ART_ASSERT(isOk, message, args) if (isOk)
#endif

#pragma warning(default:4127)

#endif
