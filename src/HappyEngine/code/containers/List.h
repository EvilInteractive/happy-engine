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
//Created: 03/11/2012

#ifndef _HE_LIST_H_
#define _HE_LIST_H_
#pragma once

#include "Creator.h"

namespace he {
    
template<typename T, typename Creator>
class List
{
public:
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef boost::function2<int, const T&, const T&> Sorter;
    typedef boost::function2<bool, const T&, const T&> Comparer;
    typedef boost::function1<bool, const T&> Pred;

    explicit List(const size_t capacity = 0);
    virtual ~List();

    inline void add(const T& element); // amortized O(1)
    inline void add(T&& element); // amortized O(1)
    inline void append(const List<T, Creator>& list); // amortized O(n) (n = parameter list count)
    inline void insert(const T& element, const size_t index); // O(n)
    inline void insert(T&& element, const size_t index); // O(n)
    
    inline void reserve(const size_t capacity);
    inline void resize(const size_t size);
    inline void trim();

    inline void remove(const T& element); // O(n)
    inline void removeAt(const size_t index); // O(1)
    inline void orderedRemove(const T& element); // O(n)
    inline void orderedRemoveAt(const size_t index); // memmove
    inline void clear();

    inline bool contains(const T& element) const; // O(n)
    inline bool find(const T& element, size_t& outIndex) const; // O(n)
    inline bool find_if(const Pred& pred, size_t& outIndex) const; // O(n)
    inline bool rfind_if(const Pred& pred, size_t& outIndex) const; // O(n)
    inline bool rfind(const T& element, size_t& outIndex) const; // O(n)
    inline bool binFind(const T& element, const Sorter& sorter, size_t& outIndex) const; // O(log(n))  binary search - only on sorted lists
    inline void sort(const Sorter& sorter); // quick sort O(n*log(n))

    inline size_t size() const { return m_Size; }

    inline const T& back() const;
    inline const T& front() const;
    inline T& back();
    inline T& front();
    inline bool empty() const { return m_Size == 0; }

    inline T& operator[](const size_t index);
    inline const T& operator[](const size_t index) const;

    inline iterator begin() { return m_Buffer; }
    inline iterator end() { return m_Buffer + m_Size; }
    inline const_iterator cbegin() const { return m_Buffer; }
    inline const_iterator cend() const { return m_Buffer + m_Size; }

    inline void forEach(const boost::function1<void, const T&>& func) const;
    inline void forEach(const boost::function1<void, T&>& func);
    inline void rForEach(const boost::function1<void, const T&>& func) const;
    inline void rForEach(const boost::function1<void, T&>& func);

    const static int CAPACITY_INCREMENT = 5;
    
private:
    void internalSort(const int begin, const int end, const Sorter& sorter);
    bool internalBinFind(const size_t begin, const size_t end, const T& element, const Sorter& sorter, size_t& outIndex) const;
    
    size_t m_Size;
    size_t m_Capacity;

    T* m_Buffer;

    //Disable default copy constructor and default assignment operator
    List(const List&);
    List& operator=(const List&);
};

template<typename T>
class PrimitiveList : public List<T, PrimitiveCreator<T>> 
{
public:
    explicit PrimitiveList(size_t capacity = 0): List<T, PrimitiveCreator<T>>(capacity) {}
    virtual ~PrimitiveList() {}
};
template<typename T>
class ObjectList : public List<T, ObjectCreator<T>> 
{
public:
    explicit ObjectList(size_t capacity = 0): List<T, ObjectCreator<T>>(capacity) {}
    virtual ~ObjectList() {}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename T, typename Creator> inline
he::List<T, Creator>::List(const size_t capacity): m_Size(0), m_Capacity(capacity), 
    m_Buffer(static_cast<T*>(he_malloc(sizeof(T) * capacity)))
{
    Creator::createArray(m_Buffer, m_Capacity);
}

template<typename T, typename Creator> inline
he::List<T, Creator>::~List()
{
    Creator::destroyArray(m_Buffer, m_Capacity);
    he_free(m_Buffer);
}

template<typename T, typename Creator> inline
void he::List<T, Creator>::add( const T& element )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to add an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    m_Buffer[m_Size++] = element;
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::add( T&& element )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to add an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    m_Buffer[m_Size++] = std::forward<T>(element);
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::append( const List<T, Creator>& list )
{
    if (m_Size + list.size() > m_Capacity)
        reserve(m_Size + list.size());
    for (size_t i(0); i < list.size(); ++i)
        m_Buffer[m_Size + i] = list.m_Buffer[i];
    resize(m_Size + list.size());
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::insert( const T& element, const size_t index )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to insert an element already in this list without a copy");
    if (m_Size == m_Capacity)
    {
        reserve(m_Capacity + CAPACITY_INCREMENT);
    }
    he_memmove(m_Buffer + index + 1, m_Buffer + index, sizeof(T) * (m_Size - index));
    m_Buffer[index] = element;
    ++m_Size;
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::insert( T&& element, const size_t index )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to insert an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    he_memmove(m_Buffer + index + 1, m_Buffer + index, sizeof(T) * (m_Size - index));
    m_Buffer[index] = std::forward<T>(element);
    ++m_Size;
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::reserve( const size_t capacity )
{
    HE_ASSERT(m_Size <= m_Capacity, "Warning reserving size smaller than used size");
    if (m_Capacity != capacity)
    {
        if (capacity < m_Capacity)
            Creator::destroyArray(m_Buffer + capacity, m_Capacity - capacity);
        m_Buffer = static_cast<T*>(he_realloc(m_Buffer, capacity * sizeof(T)));
        if (capacity > m_Capacity)
            Creator::createArray(m_Buffer + m_Capacity, capacity - m_Capacity);
        m_Capacity = capacity;
    }
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::resize( const size_t size )
{
    if (m_Capacity < size)
        reserve(size);
    m_Size = size;
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::trim()
{
    if (m_Size < m_Capacity)
    {
        reserve(m_Size);
    }
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::remove( const T& element )
{
    size_t index(0);
    if (find(element, index))
    {
        removeAt(index);
    }
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::removeAt( const size_t index )
{
    HE_IF_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size)
    {
        m_Buffer[index] = back();
        --m_Size;
    }
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::orderedRemove( const T& element )
{
    size_t index(0);
    if (find(element, index))
    {
        orderedRemoveAt(index);
    }
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::orderedRemoveAt( const size_t index )
{
    HE_IF_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size)
    {
        he_memmove(m_Buffer + index, m_Buffer + index + 1, (m_Size - index - 1) * sizeof(T));
        --m_Size;
    }
}
template<typename T, typename Creator>
void he::List<T, Creator>::clear()
{
    m_Size = 0;
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator>
bool he::List<T, Creator>::contains( const T& element ) const
{
    size_t i(0);
    return find(element, i);
}
template<typename T, typename Creator> inline
bool he::List<T, Creator>::find( const T& element, size_t& outIndex ) const
{
    bool result(false);
    for (size_t i(0); i < m_Size; ++i)
    {
        if (m_Buffer[i] == element)
        {
            outIndex = i;
            result = true;
            break;
        }
    }
    return result;
}
template<typename T, typename Creator> inline
bool he::List<T, Creator>::find_if( const Pred& condition, size_t& outIndex ) const
{
    bool result(false);
    for (size_t i(0); i < m_Size; ++i)
    {
        if (condition(m_Buffer[i]))
        {
            outIndex = i;
            result = true;
            break;
        }
    }
    return result;
}
template<typename T, typename Creator> inline
bool he::List<T, Creator>::rfind_if( const Pred& condition, size_t& outIndex ) const
{
    bool result(false);
    for (int i(m_Size - 1); i >= 0; --i)
    {
        if (condition(m_Buffer[i]))
        {
            outIndex = i;
            result = true;
            break;
        }
    }
    return result;
}
template<typename T, typename Creator> inline
bool he::List<T, Creator>::rfind( const T& element, size_t& outIndex ) const
{
    for (int i(m_Size - 1); i >= 0; --i)
    {
        if (m_Buffer[i] == element)
        {
            outIndex = i;
            return true;
        }
    }
    return false;
}
template<typename T, typename Creator> inline
bool he::List<T, Creator>::binFind( const T& element, const Sorter& sorter, size_t& outIndex ) const
{
    if (m_Size == 0)
        return false;
    return internalBinFind(0, m_Size, element, sorter, outIndex);
}
template<typename T, typename Creator>
bool he::List<T, Creator>::internalBinFind( const size_t begin, const size_t end, const T& element, const Sorter& sorter, size_t& outIndex ) const
{
    if (begin == end)
        return false;

    size_t currentIndex((begin + end) / 2);
    const T& checkElement(m_Buffer[currentIndex]);
    int check(sorter(element, checkElement));
    if (check == 0)
    {
        outIndex = currentIndex;
        return true;
    }
    else if (check == -1)
    {
        return internalBinFind(begin, currentIndex, element, sorter, outIndex);
    }
    else
    {
        return internalBinFind(currentIndex + 1, end, element, sorter, outIndex);
    }
}
template<typename T, typename Creator> inline
void he::List<T, Creator>::sort( const Sorter& sorter )
{
    if (m_Size < 2) return; // already sorted
    internalSort(0, static_cast<int>(m_Size - 1), sorter);
}
template<typename T, typename Creator>
void he::List<T, Creator>::internalSort( const int begin, const int end, const Sorter& sorter )
{
    T centerObject(m_Buffer[(begin + end) / 2]);
    int b(begin);
    int e(end);

    do 
    {
        while(sorter(m_Buffer[b], centerObject) < 0) { ++b; } // if begin is smaller than middle
        while(sorter(centerObject, m_Buffer[e]) < 0) { --e; } // if middle is smaller than end

        if(b < e)
        {
            T temp(m_Buffer[b]);
            m_Buffer[b] = m_Buffer[e];
            m_Buffer[e] = temp;
        }
        if (b <= e)
        {
            ++b;
            --e;
        }
    } while(b < e);

    if(begin < e) 
    {
        internalSort(begin, e, sorter);
    }
    if(b < end) 
    {
        internalSort(b, end, sorter);
    }
}

//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
const T& he::List<T, Creator>::front() const
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[0];
}

template<typename T, typename Creator> inline
const T& he::List<T, Creator>::back() const
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[m_Size - 1];
}
template<typename T, typename Creator>
T& he::List<T, Creator>::front()
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[0];
}
template<typename T, typename Creator>
T& he::List<T, Creator>::back()
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[m_Size - 1];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
T& he::List<T, Creator>::operator[]( const size_t index )
{
    HE_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size);
    return m_Buffer[index];
}

template<typename T, typename Creator> inline
const T& he::List<T, Creator>::operator[]( const size_t index ) const
{
    HE_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size);
    return m_Buffer[index];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::forEach( const boost::function1<void, const T&>& func ) const
{    
    const_iterator it(cbegin());
    const_iterator end(cend());
    for (; it != end; ++it)
        func(*it);
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::forEach( const boost::function1<void, T&>& func )
{    
    iterator it(begin());
    iterator endIt(end());
    for (; it != endIt; ++it)
        func(*it);
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::rForEach( const boost::function1<void, const T&>& func ) const
{    
    const_iterator it(cend());
    const_iterator end(cbegin());
    for (; it != end; --it)
        func(*(it - 1));
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Creator> inline
void he::List<T, Creator>::rForEach( const boost::function1<void, T&>& func )
{    
    iterator it(end());
    iterator end(begin());
    for (; it != end; --it)
        func(*(it - 1));
}


} //end namespace

#endif
