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

} //end namespace

#endif
