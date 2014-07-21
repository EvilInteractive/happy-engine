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
//Created: 2014/07/21
#include "HappyPCH.h" 

#include "HappyFunctions.h"

namespace he {

int hevsnprintf_s( char* buffer, const size_t buffSize, const char* format, va_list& vaList )
{
    if (buffSize > 0)
    {
        const int actualCharsWritten(vsnprintf(buffer, buffSize, format, vaList));
        return he::clamp<int>(actualCharsWritten, 0, buffSize);
    }
    return 0;
}

int hevsnprintf( char* buffer, const size_t buffSize, const char* format, va_list& vaList )
{
    int charsWritten(0);
    if (buffer != nullptr)
    {
        if (buffSize > 0)
        {
            charsWritten = vsnprintf(buffer, buffSize, format, vaList);
            HE_ASSERT(charsWritten >= 0 && charsWritten <= static_cast<int>(buffSize), "hevsnprintf could not write all characters to the buffer.\nFormat was %s\nTried to write %d characters to a buffer size of %u", format, charsWritten, buffSize);
            return he::clamp<int>(charsWritten, 0, buffSize);
        }
    }
    else
    {
        charsWritten = vsnprintf(nullptr, 0, format, vaList);
    }
    return charsWritten;
}

int hesnprintf_s( char* buffer, const size_t buffSize, const char* format, ... )
{
    va_list list;
    va_start(list, format);
    const int charsWritten(hevsnprintf_s(buffer, buffSize, format, list));
    va_end(list);
    return charsWritten;
}

int hesnprintf( char* buffer, const size_t buffSize, const char* format, ... )
{
    va_list list;
    va_start(list, format);
    const int charsWritten(hevsnprintf(buffer, buffSize, format, list));
    va_end(list);
    return charsWritten;
}

} //end namespace
