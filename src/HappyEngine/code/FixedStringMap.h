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
    typedef std::unordered_map<FixedString, T, FixedStringHasher> _InternalFixedString;
public:
    FixedStringMap() : std::unordered_map<FixedString, T, FixedStringHasher>() {}
    // do nothing special in here!

    inline void forEach(const std::function<void(const FixedString&, T&)>& func);
    inline void forEach(const std::function<void(const FixedString&, const T&)>& func) const;

    inline void setAt(const he::FixedString& key, const T& value);
    inline void setAt(const he::FixedString& key, T&& value);

    inline const T* find(const he::FixedString& key) const;
    inline T* find(const he::FixedString& key);

private:
    FixedStringMap(const FixedStringMap&);
    FixedStringMap& operator=(FixedStringMap&);
};

template<typename T>
void he::FixedStringMap<T>::forEach( const std::function<void(const FixedString&, T&)>& func )
{
    typename std::unordered_map<FixedString, T, FixedStringHasher>::iterator it(this->begin());
    typename std::unordered_map<FixedString, T, FixedStringHasher>::iterator end(this->end());
    for (; it != end; ++it)
        func(it->first, it->second);
}

template<typename T>
void he::FixedStringMap<T>::forEach( const std::function<void(const FixedString&, const T&)>& func ) const
{
    typename std::unordered_map<FixedString, T, FixedStringHasher>::const_iterator it(this->cbegin());
    typename std::unordered_map<FixedString, T, FixedStringHasher>::const_iterator end(this->cend());
    for (; it != end; ++it)
        func(it->first, it->second);
}

template<typename T>
void he::FixedStringMap<T>::setAt( const he::FixedString& key, const T& value )
{
    operator[](key) = value;
}

template<typename T>
void he::FixedStringMap<T>::setAt( const he::FixedString& key, T&& value )
{
    operator[](key) = std::forward<T>(value);
}

template<typename T>
const T* he::FixedStringMap<T>::find( const he::FixedString& key ) const
{
    const_iterator it(_InternalFixedString::find(key));
    if (it != cend())
    {
        return &it->second;
    }
    return nullptr;
}

template<typename T>
T* he::FixedStringMap<T>::find( const he::FixedString& key )
{
    iterator it(_InternalFixedString::find(key));
    if (it != end())
    {
        return &it->second;
    }
    return nullptr;
}

} //end namespace

#endif
