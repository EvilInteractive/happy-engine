//HappyEngine Copyright (C) 2011 - 2013  Evil Interactive
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
//Created: 2013/10/29

#ifndef _HE_Map_H_
#define _HE_Map_H_
#pragma once

#include <unordered_map>

namespace he {

template<typename KEY, typename VALUE>
class Map : public std::unordered_map<KEY, VALUE>
{
public:
    Map() : std::unordered_map<KEY, VALUE>() {}
    // do nothing special in here!


    inline void forEach(const boost::function1<void, const std::pair<const KEY, VALUE>&>& func) const;
    inline void forEach(const boost::function1<void, std::pair<const KEY, VALUE>&>& func);

private:
    Map(const Map&);
    Map& operator=(Map&);
};

} //end namespace

#include "Map.inl"

#endif
