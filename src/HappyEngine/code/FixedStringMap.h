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
//Created: 2013/03/19

#ifndef _HE_FixedStringMap_H_
#define _HE_FixedStringMap_H_
#pragma once

#include <unordered_map>
#include <FixedString.h>

namespace he {

class FixedStringHasher
{
public:
    size_t operator()(const FixedString& str) const
    {
        return reinterpret_cast<size_t>(str.getHandle());
    }
};

template<typename T>
class FixedStringMap : public std::unordered_map<FixedString, T, FixedStringHasher>
{
public:
    FixedStringMap() : std::unordered_map<FixedString, T, FixedStringHasher>() {}
    // do nothing special in here!


    inline void forEach(const boost::function1<void, const std::pair<const FixedString, T>&>& func) const;
    inline void forEach(const boost::function1<void, std::pair<const FixedString, T>&>& func);

private:
    FixedStringMap(const FixedStringMap&);
    FixedStringMap& operator=(FixedStringMap&);
};

template<typename T>
void he::FixedStringMap<T>::forEach( const boost::function1<void, std::pair<const FixedString, T>&>& func )
{
    typename std::unordered_map<FixedString, T, FixedStringHasher>::iterator it(this->begin());
    typename std::unordered_map<FixedString, T, FixedStringHasher>::iterator end(this->end());
    for (; it != end; ++it)
        func(*it);
}

template<typename T>
void he::FixedStringMap<T>::forEach( const boost::function1<void, const std::pair<const FixedString, T>&>& func ) const
{
    typename std::unordered_map<FixedString, T, FixedStringHasher>::const_iterator it(this->cbegin());
    typename std::unordered_map<FixedString, T, FixedStringHasher>::const_iterator end(this->cend());
    for (; it != end; ++it)
        func(*it);
}

} //end namespace

#endif
