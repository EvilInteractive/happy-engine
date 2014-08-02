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
//Created: 2014/06/21

#ifndef _HE_HappyFunctions_H_
#define _HE_HappyFunctions_H_
#pragma once

namespace he {

#ifdef HE_DEBUG
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

// Arguments defined as -argument value
inline const char* getProgramArgumentValue(const int argc, const char* const * const argv, const char* const argument)
{
    for (int i(0); i < argc; ++i)
    {
        if (*argv[i] == '-' && strcmp(argv[i]+1, argument) == 0 && i+1 != argc)
        {
            return argv[i+1];
        }
    }
    return nullptr;
}

// None of the following methods will crash if there is not enough space in the buffer
// the differnce between the _s version and the none _s version is that the none _s version
// will give an assert when it goes out of bounds.
// Please keep in mind that this method will fill in the last character when the buffer is flooded
// So always pass as buffSize one byte less than allocated, or the '\0' will be missing
// Returns how many actual chars have been written
int HAPPY_ENTRY hevsnprintf_s(char* buffer, const size_t buffSize, const char* format, va_list& vaList);
// This one can be used to get the needed size, if you pass in nullptr for the buffer
int HAPPY_ENTRY hevsnprintf(char* buffer, const size_t buffSize, const char* format, va_list& vaList);
int HAPPY_ENTRY hesnprintf_s(char* buffer, const size_t buffSize, const char* format, ...);
// This one can be used to get the needed size, if you pass in nullptr for the buffer
int HAPPY_ENTRY hesnprintf(char* buffer, const size_t buffSize, const char* format, ...);

#ifndef stricmp
int stricmp(const char* s1, const char* s2);
#endif

} //end namespace

#endif
