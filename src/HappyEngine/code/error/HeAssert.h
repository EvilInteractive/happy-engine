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
//Created: 13/06/2011
//Updated assert: 04/09/2011

#ifndef _HE_ASSERT_H_
#define _HE_ASSERT_H_
#pragma once

namespace he {
namespace err {
namespace details {

enum AssertType
{
    AssertType_Code,
    AssertType_Art
};

#ifdef HE_DEBUG
void happyAssert(AssertType type, const char* file, const char* func, int line, const char* message);
void happyAssert(AssertType type, const char* file, const char* func, int line, const char* message);
#endif

} } } //end namespace

#ifdef HE_DEBUG
#define HE_ASSERT(isOk, message, ...) if (!(isOk)) LOG(he::LogType_ProgrammerAssert, message, ##__VA_ARGS__)
#define HE_IF_ASSERT(isOk, message, ...) if (!(isOk)) { LOG(he::LogType_ProgrammerAssert, message, ##__VA_ARGS__); } else 
#define HE_ART_ASSERT(isOk, message, ...) if (!(isOk)) LOG(he::LogType_ArtAssert, message, ##__VA_ARGS__)
#define HE_IF_ART_ASSERT(isOk, message, ...) if (!(isOk)) { LOG(he::LogType_ArtAssert, message, ##__VA_ARGS__); } else
#define HE_NOT_IMPLEMENTED LOG(he::LogType_ProgrammerAssert, "Method not implemented! %s", __HE_FUNCTION__);
#else
#define HE_ASSERT(...) {}
#define HE_IF_ASSERT(...) {}
#define HE_ART_ASSERT(...) {}
#define HE_IF_ART_ASSERT(isOk, ...) if (isOk)
#define HE_NOT_IMPLEMENTED
#endif
#define HE_COMPILE_ASSERT(isOk, message) static_assert(isOk, message)

#endif
