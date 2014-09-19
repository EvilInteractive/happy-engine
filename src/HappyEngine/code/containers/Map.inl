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
//Created: 2013/10/29

namespace he {
    
template<typename KEY, typename VALUE, typename HASH>
void he::Map<KEY, VALUE, HASH>::forEach( const std::function<void(std::pair<const KEY, VALUE>&)>& func )
{
    typename std::unordered_map<KEY, VALUE, HASH>::iterator it(this->begin());
    typename std::unordered_map<KEY, VALUE, HASH>::iterator end(this->end());
    for (; it != end; ++it)
    func(*it);
}

template<typename KEY, typename VALUE, typename HASH>
void he::Map<KEY, VALUE, HASH>::forEach( const std::function<void(const std::pair<const KEY, VALUE>&)>& func ) const
{
    typename std::unordered_map<KEY, VALUE, HASH>::const_iterator it(this->cbegin());
    typename std::unordered_map<KEY, VALUE, HASH>::const_iterator end(this->cend());
    for (; it != end; ++it)
    func(*it);
}

} //end namespace
