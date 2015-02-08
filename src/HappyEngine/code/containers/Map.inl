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

template<typename TKey, typename TValue, typename THash, typename TCompare>
struct he::Map<TKey, TValue, THash, TCompare>::TBucket
{
    TBucket() {}
    ~TBucket() {}

    struct KeyValuePair
    {
        KeyValuePair() {}
        KeyValuePair(const TKey key, TValue&& value) : m_Key(std::move(key)), m_Value(std::forward<TValue>(value)) {}
        KeyValuePair(KeyValuePair&& other) : m_Key(std::move(other.m_Key)), m_Value(std::move(other.m_Value)) {}
        KeyValuePair& operator=(KeyValuePair&& other) { m_Key = std::move(other.m_Key); m_Value = std::move(other.m_Value); return *this; }
        TKey m_Key;
        TValue m_Value;
    private:
        KeyValuePair(const KeyValuePair& other);
        KeyValuePair& operator=(const KeyValuePair& other);
    };
    HE_FORCEINLINE static int compare(const TKey& a, const KeyValuePair& b)
    {
        return TCompare::compare(a, b.m_Key);
    }

    he::ObjectList<KeyValuePair> m_Values;
};

template<typename TKey, typename TValue, typename THash, typename TCompare>
he::Map<TKey, TValue, THash, TCompare>::Map( const size_t bucketCount /*= 37*/ )
    : m_Buckets(HENewArray(TBucket, bucketCount))
    , m_BucketCount(bucketCount)
{

}

template<typename TKey, typename TValue, typename THash, typename TCompare>
he::Map<TKey, TValue, THash, TCompare>::~Map()
{
    HEDeleteArray(m_Buckets);
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
he::Map<TKey, TValue, THash, TCompare>::Map( Map&& other ) 
    : m_Buckets(other.m_Buckets)
    , m_BucketCount(other.m_BucketCount)
{
    other.m_Buckets = nullptr;
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
Map<TKey, TValue, THash, TCompare>& he::Map<TKey, TValue, THash, TCompare>::operator=( Map&& other )
{
    HEDeleteArray(m_Buckets);
    m_Buckets = other.m_Buckets;
    m_BucketCount = other.m_BucketCount;
    other.m_Buckets = nullptr;

    return *this;
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
void he::Map<TKey, TValue, THash, TCompare>::insert( const TKey& key, TValue&& value )
{
    const size_t bucketIndex(THash::hash(key) % m_BucketCount);
    TBucket& bucket(m_Buckets[bucketIndex]);
    size_t currentIndex(0);
    if (bucket.m_Values.binFind<TKey>(key, TBucket::compare, currentIndex))
    {
        bucket.m_Values[currentIndex].m_Value = std::forward<TValue>(value);
    }
    else
    {
        bucket.m_Values.insert(TBucket::KeyValuePair(key, std::forward<TValue>(value)), currentIndex);
    }
}

template<typename TKey, typename TValue, typename THash /*= HashSimple<TKey>*/, typename TCompare /*= CompareSimple<TKey>*/>
bool he::Map<TKey, TValue, THash, TCompare>::remove( const TKey& key )
{
    const size_t bucketIndex(THash::hash(key) % m_BucketCount);
    TBucket& bucket(m_Buckets[bucketIndex]);
    size_t currentIndex(0);
    if (bucket.m_Values.binFind<TKey>(key, TBucket::compare, currentIndex))
    {
        bucket.m_Values.orderedRemoveAt(currentIndex);
        return true;
    }
    return false;
}

template<typename TKey, typename TValue, typename THash /*= HashSimple<TKey>*/, typename TCompare /*= CompareSimple<TKey>*/>
void he::Map<TKey, TValue, THash, TCompare>::clear()
{
    const size_t buckets(m_BucketCount);
    for (size_t i(0); i < buckets; ++i)
    {
        m_Buckets[i].m_Values.clear();
    }
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
TValue* he::Map<TKey, TValue, THash, TCompare>::find( const TKey& key ) const
{
    const size_t bucketIndex(THash::hash(key) % m_BucketCount);
    TBucket& bucket(m_Buckets[bucketIndex]);
    size_t currentIndex(0);
    if (bucket.m_Values.binFind<TKey>(key, TBucket::compare, currentIndex))
    {
        return &bucket.m_Values[currentIndex].m_Value;
    }
    return nullptr;
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
bool he::Map<TKey, TValue, THash, TCompare>::contains( const TKey& key ) const
{
    return find(key) != nullptr;
}

template<typename TKey, typename TValue, typename THash /*= HashSimple<TKey>*/, typename TCompare /*= CompareSimple<TKey>*/>
TValue& he::Map<TKey, TValue, THash, TCompare>::operator[]( const TKey& key )
{
    const size_t bucketIndex(THash::hash(key) % m_BucketCount);
    TBucket& bucket(m_Buckets[bucketIndex]);
    size_t currentIndex(0);
    if (!bucket.m_Values.binFind<TKey>(key, TBucket::compare, currentIndex))
    {
        bucket.m_Values.insert(TBucket::KeyValuePair(key, TValue()), currentIndex);
    }
    return bucket.m_Values[currentIndex].m_Value;
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
void he::Map<TKey, TValue, THash, TCompare>::forEach( const std::function<void(const TKey&, TValue&)>& func )
{
    const size_t buckets(m_BucketCount);
    for (size_t i(0); i < buckets; ++i)
    {
        m_Buckets[i].m_Values.forEach([&func](TBucket::KeyValuePair& kvp) { func(kvp.m_Key, kvp.m_Value); } );
    }
}

template<typename TKey, typename TValue, typename THash, typename TCompare>
void he::Map<TKey, TValue, THash, TCompare>::forEach( const std::function<void(const TKey&, const TValue&)>& func ) const
{
    const size_t buckets(m_BucketCount);
    for (size_t i(0); i < buckets; ++i)
    {
        m_Buckets[i].m_Values.forEach([&func](const TBucket::KeyValuePair& kvp) { func(kvp.m_Key, kvp.m_Value); } );
    }
}

} //end namespace
