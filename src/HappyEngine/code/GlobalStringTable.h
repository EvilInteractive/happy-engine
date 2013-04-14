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
//Created: 2013/03/15

#ifndef _HE_GlobalStringTable_H_
#define _HE_GlobalStringTable_H_
#pragma once

#include <unordered_set>

namespace he {

class GlobalStringTable : public Singleton<GlobalStringTable>
{
    class HeStringHash
    {
    public:
        size_t operator()(const char* str) const
        {
            size_t hash(0);
            while (*str)
            {
                hash = hash * 101 + *str++;
            }
            return hash;
        }
    };

    class HeStringComp
    {
    public:
        bool operator()(const char* a, const char* b) const
        {
            return strcmp(a, b) == 0;
        }
    };

    typedef std::unordered_set<const char*, HeStringHash, HeStringComp> Map;
public:
    GlobalStringTable();
    virtual ~GlobalStringTable();

    FixedString add(const char* const str, const int len = -1);

private:
    Map m_HashMap;
    std::unordered_set<std::string> tes;

    //Disable default copy constructor and default assignment operator
    GlobalStringTable(const GlobalStringTable&);
    GlobalStringTable& operator=(const GlobalStringTable&);
};

} //end namespace

#endif
