//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 13/03/2013

#ifndef _HE_FixedString_H_
#define _HE_FixedString_H_
#pragma once

namespace he {
class GlobalStringTable;
class HAPPY_ENTRY FixedString
{
friend class GlobalStringTable;
public:
    FixedString(): m_Handle(nullptr) {}
    FixedString(const FixedString& other);
    FixedString& operator=(const FixedString& other);
    ~FixedString();

    // Getters
    const char* c_str() const { return m_Handle; }
    const char* getHandle() const { return m_Handle; }

    // Operators
    bool operator==(const FixedString& other) const { return m_Handle == other.m_Handle; }
    bool operator!=(const FixedString& other) const { return m_Handle != other.m_Handle; }

    // Static
    static FixedString fromString(const char* const str, const int len = -1);

private:
    const char* m_Handle;

    explicit FixedString(const char* const str);
};

} //end namespace

#endif
