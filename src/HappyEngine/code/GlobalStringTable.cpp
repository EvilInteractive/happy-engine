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
#include "HappyPCH.h" 

#include "GlobalStringTable.h"

namespace he {

GlobalStringTable::GlobalStringTable()
{
}


GlobalStringTable::~GlobalStringTable()
{
    Map::iterator it(m_HashMap.begin());
    Map::iterator end(m_HashMap.end());
    for (; it != end; ++it)
    {
        he_free(const_cast<char*>(*it));
    }
}

he::FixedString GlobalStringTable::add( const char* const str, const int length )
{
    Map::const_iterator it(m_HashMap.find(str));
    if (it == m_HashMap.cend())
    {
        const int len(length == -1? strlen(str) : length);
        char* const myStr(static_cast<char*>(he_malloc((len + 1) * sizeof(char))));
        he_memcpy(myStr, str, len * sizeof(char));
        myStr[len] = '\0';
        m_HashMap.insert(myStr);
        return FixedString(myStr);
    }
    else
    {
        HE_ASSERT(strcmp(str, *it) == 0, "Hash clash occurd! %s != %s", str, *it);
        return FixedString(*it);
    }
}

} //end namespace
