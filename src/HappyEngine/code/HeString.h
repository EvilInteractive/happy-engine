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
//Created: 2013/03/09

#ifndef _HE_String_H_
#define _HE_String_H_
#pragma once

#include <string>

namespace he {

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > StdString;
class String : public StdString
{
public:
    String();
    ~String();

    String(const char* charBuffer, const int len = -1);
    String(const wchar_t* charBuffer);
    String(he::String&& other);
    String(std::string&& other);
    String(const he::String& other);
    String& operator=(he::String&& other);
    String& operator=(const he::String& other);
    
    he::String clone() const;
    uint32 hash() const;

    // Operators
    bool operator==(const he::String& other) const;
    bool operator!=(const he::String& other) const;
    bool operator<(const he::String& other) const;
    bool operator==(const char* const other) const;
    bool operator!=(const char* const other) const;
    bool operator<(const char* const other) const;

    String& operator+=(const he::String& other);
    String& operator+=(he::String&& other);
    String& operator+=(const char* const other);
    String operator+(const he::String& other) const;
    String operator+(he::String&& other) const;
    String operator+(const char* const other) const;
};

void toLower(char* const buff);

} //end namespace

#endif
