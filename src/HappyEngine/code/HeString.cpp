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
#include "HappyPCH.h" 

#include "HeString.h"

namespace he {

void toLower(char* const buff)
{
    char* chr(buff);
    while(*chr)
    {
        char& c(*chr);
        c = static_cast<char>(tolower(c));
        ++chr;
    }
}

String::String()
{

}

String::String( const char* charBuffer, const int len /*= -1*/ )
    : StdString(charBuffer, len)
{

}

String::String( const wchar_t* charBuffer )
    : StdString()
{
    const size_t sizeNeeded(wcstombs(NULL, charBuffer, 0));
    resize(sizeNeeded);
    wcstombs(&operator[](0), charBuffer, sizeNeeded);
}

String::String( const he::String& other )
    : StdString(other)
{

}

String::String( he::String&& other )
    : StdString(std::forward<he::String>(other))
{

}

String::String( std::string&& other )
    : StdString(std::forward<he::StdString>(other))
{

}

String::~String()
{

}

String& String::operator=( he::String&& other )
{
    StdString::operator=(std::forward<he::String>(other));
    return *this;
}

String& String::operator=( const he::String& other )
{
    StdString::operator=(other);
    return *this;
}

bool String::operator!=( const he::String& other ) const
{
    return compare(other) != 0;
}

bool String::operator==( const he::String& other ) const
{
    return compare(other) == 0;
}

bool String::operator<( const he::String& other ) const
{
    return compare(other) < 0;
}

bool String::operator!=( const char* const other ) const
{
    return compare(other) != 0;
}

bool String::operator==( const char* const other ) const
{
    return compare(other) == 0;
}

bool String::operator<( const char* const other ) const
{
    return compare(other) < 0;
}

he::String String::clone() const
{
    return he::String(c_str(), length());
}

he::uint32 String::hash() const
{
    const char* buff(c_str());
    if (buff)
    {
        uint32 hash(buff[0]);
        while (!(*++buff))
        {
            hash = hash * 101 + *buff;
        }
    }
    return 0;
}

String& String::operator+=( const he::String& other )
{
    StdString::operator+=(other);
    return *this;
}

String& String::operator+=( he::String&& other )
{
    StdString::operator+=(std::forward<he::String>(other));
    return *this;
}

String& String::operator+=( const char* const other )
{
    StdString::operator+=(other);
    return *this;
}

he::String String::operator+( const he::String& other ) const
{
    return clone() += other;
}

he::String String::operator+( he::String&& other ) const
{
    return clone() += other;
}

he::String String::operator+( const char* const other ) const
{
    return clone() += other;
}

} //end namespace
