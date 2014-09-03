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
//Created: 2014/01/08

#ifndef _HE_HAPPY_GUID_H_
#define _HE_HAPPY_GUID_H_
#pragma once

namespace he {

struct HAPPY_ENTRY Guid
{
    Guid();
    ~Guid() {}
    explicit Guid(const char* const guid);

    he::String toString() const;
    void toString(char* const charBuffer) const; // charBuffer must be at least s_CharbufferSize in size

    static Guid generateGuid();

    static const int s_CharbufferSize = 37;

    bool operator==(const Guid& other) const;
    bool operator!=(const Guid& other) const;

    uint32 m_Data1;
    // -
    uint16 m_Data2;
    // -
    uint16 m_Data3;
    // -
    uint16 m_Data4;
    // -
    uint16 m_Data5;
    uint32 m_Data6;
};

} //end namespace


#endif
