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

#ifndef _HE_Map_H_
#define _HE_Map_H_
#pragma once

namespace he {
    
template<typename T, bool U = std::is_scalar<T>::value>
struct HashSimple;

template<typename T>
struct HashSimple<T, true>
{
    static_assert(sizeof(T) <= sizeof(size_t), "Do not use HashSimple for this simple type, it is too large!");
    HE_FORCEINLINE static size_t hash(const T& key)
    {
        return (size_t)(key); // keep c-style cast, needs to traverse all the cast types
    }
};

template<typename T>
struct HashSimple<T, false>
{
    HE_FORCEINLINE static size_t hash(const T& key)
    {
        return key.hash();
    }
};

template<>
struct HashSimple<he::String, false>
{
    HE_FORCEINLINE static size_t hash(const he::String& key)
    {
        return he::hash(0, key);
    }
};

template<typename T, bool U = std::is_scalar<T>::value>
struct CompareSimple;

template<typename T>
struct CompareSimple<T, true>
{
    HE_FORCEINLINE static int compare(const T& a, const T& b)
    {
        return b - a;
    }
};

template<typename T>
struct CompareSimple<T, false>
{
    HE_FORCEINLINE static int compare(const T& a, const T& b)
    {
        return a < b ? -1 : (a == b ? 0 : 1);
    }
};

template<typename TKey, typename TValue, typename THash = HashSimple<TKey>, typename TCompare = CompareSimple<TKey>>
class Map
{
public:
    explicit Map(const size_t bucketCount = 37);
    ~Map();

    Map(Map&& other);
    Map& operator=(Map&& other);

    inline void insert(const TKey& key, TValue&& value);

    inline bool remove(const TKey& key);
    inline void clear();

    inline TValue* find(const TKey& key) const;
    inline bool contains(const TKey& key) const;

    inline TValue& operator[](const TKey& key);

    inline void forEach(const std::function<void(const TKey&, const TValue&)>& func) const;
    inline void forEach(const std::function<void(const TKey&, TValue&)>& func);

private:
    Map(const Map&);
    Map& operator=(const Map&);

    struct TBucket;
    TBucket* m_Buckets;
    size_t m_BucketCount;
};

} //end namespace

#include "Map.inl"

#endif
