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
//Created: 03/11/2012

#ifndef _HE_LIST_H_
#define _HE_LIST_H_
#pragma once

#include "Creator.h"

namespace he {
    
template<typename T, typename Allocator>
class List
{
public:
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef std::function<int(const T&, const T&)> Sorter;
    typedef std::function<bool(const T&, const T&)> Comparer;
    typedef std::function<bool(const T&)> Pred;

    explicit List(const size_t capacity = 0);
    List(List<T, Allocator>&& other);
    List<T, Allocator>& operator=(List<T, Allocator>&& other);
    virtual ~List();

    inline void add(const T& element); // amortized O(1)
    inline void add(T&& element); // amortized O(1)
    inline void append(const List<T, Allocator>& list); // amortized O(n) (n = parameter list count)
    inline void insert(const T& element, const size_t index); // O(n)
    inline void insert(T&& element, const size_t index); // O(n)
    
    inline void reserve(const size_t capacity);
    inline void resize(const size_t size);
    inline void trim();

    inline bool remove(const T& element); // O(n)
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

    inline void pop();

    inline T& operator[](const size_t index);
    inline const T& operator[](const size_t index) const;

    inline iterator begin() { return m_Buffer; }
    inline iterator end() { return m_Buffer + m_Size; }
    inline const_iterator cbegin() const { return m_Buffer; }
    inline const_iterator cend() const { return m_Buffer + m_Size; }

    inline void forEach(const std::function<void(const T&)>& func) const;
    inline void forEach(const std::function<void(T&)>& func);
    inline void rForEach(const std::function<void(const T&)>& func) const;
    inline void rForEach(const std::function<void(T&)>& func);

    const static int CAPACITY_INCREMENT = 5;
    
private:
    void internalSort(const int begin, const int end, const Sorter& sorter);
    bool internalBinFind(const size_t begin, const size_t end, const T& element, const Sorter& sorter, size_t& outIndex) const;
    
    size_t m_Size;
    size_t m_Capacity;

    T* m_Buffer;

#ifdef HE_DEBUG
    mutable bool m_IsTraversing;
#endif

    //Disable default copy constructor and default assignment operator
    List<T, Allocator>(const List<T, Allocator>&);
    List<T, Allocator>& operator=(const List<T, Allocator>&);
};

template<typename T>
class PrimitiveList : public List<T, PrimitiveObjectAllocator<T>> 
{
public:
    explicit PrimitiveList(size_t capacity = 0): List<T, PrimitiveObjectAllocator<T>>(capacity) {}
    virtual ~PrimitiveList() {}

    PrimitiveList(PrimitiveList<T>&& other) : List<T, PrimitiveObjectAllocator<T>>(std::forward<List<T, PrimitiveObjectAllocator<T>>>(other)) 
    {
    }
    PrimitiveList<T>& operator=(PrimitiveList<T>&& other) 
    { 
        List<T, PrimitiveObjectAllocator<T>>::operator=(std::forward<List<T, PrimitiveObjectAllocator<T>>>(other));
        return *this;
    }
};
template<typename T>
class ObjectList : public List<T, ObjectAllocator<T>> 
{
public:
    explicit ObjectList(size_t capacity = 0): List<T, ObjectAllocator<T>>(capacity) {}
    virtual ~ObjectList() {}

    ObjectList(ObjectList<T>&& other) : List<T, ObjectAllocator<T>>(std::forward<List<T, ObjectAllocator<T>>>(other)) 
    {
    }
    ObjectList<T>& operator=(ObjectList<T>&& other)
    { 
        List<T, ObjectAllocator<T>>::operator=(std::forward<List<T, ObjectAllocator<T>>>(other));
        return *this;
    }
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename T, typename Allocator> inline
he::List<T, Allocator>::List(const size_t capacity): m_Size(0), m_Capacity(capacity), 
    m_Buffer(Allocator::allocate(capacity))
#ifdef HE_DEBUG
    , m_IsTraversing(false)
#endif
{
}

template<typename T, typename Allocator>
he::List<T, Allocator>::List( List<T, Allocator>&& other )
    : m_Size(other.m_Size)
    , m_Capacity(other.m_Capacity)
    , m_Buffer(other.m_Buffer)
{
    other.m_Size = 0;
    other.m_Capacity = 0;
    other.m_Buffer = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>& he::List<T, Allocator>::operator=( List<T, Allocator>&& other )
{
    Allocator::deallocate(m_Buffer);
    m_Size = other.m_Size;
    m_Capacity = other.m_Capacity;
    m_Buffer = other.m_Buffer;
    other.m_Size = 0;
    other.m_Capacity = 0;
    other.m_Buffer = nullptr;

    return *this;
}

template<typename T, typename Allocator> inline
he::List<T, Allocator>::~List()
{
    Allocator::deallocate(m_Buffer);
}

template<typename T, typename Allocator> inline
void he::List<T, Allocator>::add( const T& element )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to add an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    m_Buffer[m_Size++] = element;
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::add( T&& element )
{
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to add an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    m_Buffer[m_Size++] = std::forward<T>(element);
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::append( const List<T, Allocator>& list )
{
    if (m_Size + list.size() > m_Capacity)
        reserve(m_Size + list.size());
    for (size_t i(0); i < list.size(); ++i)
        m_Buffer[m_Size + i] = list.m_Buffer[i];
    resize(m_Size + list.size());
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::insert( const T& element, const size_t index )
{
    HE_ASSERT(!m_IsTraversing, "Inserting element into list, but we are currently traversing the list! Bad things will happen.");
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to insert an element already in this list without a copy");
    if (m_Size == m_Capacity)
    {
        reserve(m_Capacity + CAPACITY_INCREMENT);
    }
    he_memmove(m_Buffer + index + 1, m_Buffer + index, sizeof(T) * (m_Size - index));
    m_Buffer[index] = element;
    ++m_Size;
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::insert( T&& element, const size_t index )
{
    HE_ASSERT(!m_IsTraversing, "Inserting element into list, but we are currently traversing the list! Bad things will happen.");
    HE_ASSERT(!(&element >= begin() && &element < end()), "List memcorruption can occur! trying to insert an element already in this list without a copy");
    if (m_Size == m_Capacity)
        reserve(m_Capacity + CAPACITY_INCREMENT);
    he_memmove(m_Buffer + index + 1, m_Buffer + index, sizeof(T) * (m_Size - index));
    m_Buffer[index] = std::forward<T>(element);
    ++m_Size;
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::reserve( const size_t capacity )
{
    HE_ASSERT(m_Size <= m_Capacity, "Warning reserving size smaller than used size");
    if (m_Capacity != capacity)
    {
        m_Buffer = Allocator::reallocate(m_Buffer, m_Capacity, capacity);
        m_Capacity = capacity;
    }
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::resize( const size_t size )
{
    HE_ASSERT(!m_IsTraversing || m_Size <= size, "Resizing list, but we are currently traversing the list! Bad things will happen.");
    if (m_Capacity < size)
        reserve(size);
    m_Size = size;
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::trim()
{
    if (m_Size < m_Capacity)
    {
        reserve(m_Size);
    }
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator>
void he::List<T, Allocator>::pop()
{
    HE_ASSERT(!m_IsTraversing, "Popping element from list, but we are currently traversing the list! Bad things will happen.");
    HE_IF_ASSERT(m_Size > 0, "Index out of bounds! cannot pop an item of the list when there are not items!")
    {
        --m_Size;
    }
}

template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::remove( const T& element )
{
    HE_ASSERT(!m_IsTraversing, "Removing element from list, but we are currently traversing the list! Bad things will happen.");
    size_t index(0);
    if (find(element, index))
    {
        removeAt(index);
        return true;
    }
    return false;
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::removeAt( const size_t index )
{
    HE_ASSERT(!m_IsTraversing, "Removing element from list, but we are currently traversing the list! Bad things will happen.");
    HE_IF_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size)
    {
        m_Buffer[index] = back();
        --m_Size;
    }
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::orderedRemove( const T& element )
{
    HE_ASSERT(!m_IsTraversing, "Removing element from list, but we are currently traversing the list! Bad things will happen.");
    size_t index(0);
    if (find(element, index))
    {
        orderedRemoveAt(index);
    }
}
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::orderedRemoveAt( const size_t index )
{
    HE_ASSERT(!m_IsTraversing, "Removing element from list, but we are currently traversing the list! Bad things will happen.");
    HE_IF_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size)
    {
        he_memmove(m_Buffer + index, m_Buffer + index + 1, (m_Size - index - 1) * sizeof(T));
        --m_Size;
    }
}
template<typename T, typename Allocator>
void he::List<T, Allocator>::clear()
{
    HE_ASSERT(!m_IsTraversing, "Clearing list, but we are currently traversing the list! Bad things will happen.");
    m_Size = 0;
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator>
bool he::List<T, Allocator>::contains( const T& element ) const
{
    size_t i(0);
    return find(element, i);
}
template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::find( const T& element, size_t& outIndex ) const
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
template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::find_if( const Pred& condition, size_t& outIndex ) const
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
template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::rfind_if( const Pred& condition, size_t& outIndex ) const
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
template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::rfind( const T& element, size_t& outIndex ) const
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
template<typename T, typename Allocator> inline
bool he::List<T, Allocator>::binFind( const T& element, const Sorter& sorter, size_t& outIndex ) const
{
    if (m_Size == 0)
        return false;
    return internalBinFind(0, m_Size, element, sorter, outIndex);
}
template<typename T, typename Allocator>
bool he::List<T, Allocator>::internalBinFind( const size_t begin, const size_t end, const T& element, const Sorter& sorter, size_t& outIndex ) const
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
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::sort( const Sorter& sorter )
{
    HE_ASSERT(!m_IsTraversing, "Sorting list, but we are currently traversing the list! Bad things will happen.");
    if (m_Size < 2) return; // already sorted
    internalSort(0, static_cast<int>(m_Size - 1), sorter);
}
template<typename T, typename Allocator>
void he::List<T, Allocator>::internalSort( const int begin, const int end, const Sorter& sorter )
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
template<typename T, typename Allocator> inline
const T& he::List<T, Allocator>::front() const
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[0];
}

template<typename T, typename Allocator> inline
const T& he::List<T, Allocator>::back() const
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[m_Size - 1];
}
template<typename T, typename Allocator>
T& he::List<T, Allocator>::front()
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[0];
}
template<typename T, typename Allocator>
T& he::List<T, Allocator>::back()
{
    HE_ASSERT(m_Size > 0, "No elements in List! Index out of bounds exception");
    return m_Buffer[m_Size - 1];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
T& he::List<T, Allocator>::operator[]( const size_t index )
{
    HE_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size);
    return m_Buffer[index];
}

template<typename T, typename Allocator> inline
const T& he::List<T, Allocator>::operator[]( const size_t index ) const
{
    HE_ASSERT(index < m_Size, "Index out of bounds! getting %d from #%d elements", index, m_Size);
    return m_Buffer[index];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::forEach( const std::function<void(const T&)>& func) const
{    
#ifdef HE_DEBUG
    m_IsTraversing = true;
#endif
    const_iterator it(cbegin());
    const_iterator end(cend());
    for (; it != end; ++it)
        func(*it);
#ifdef HE_DEBUG
    m_IsTraversing = false;
#endif
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::forEach( const std::function<void(T&)>& func )
{    
#ifdef HE_DEBUG
    m_IsTraversing = true;
#endif
    iterator it(begin());
    iterator endIt(end());
    for (; it != endIt; ++it)
        func(*it);
#ifdef HE_DEBUG
    m_IsTraversing = false;
#endif
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::rForEach( const std::function<void(const T&)>& func ) const
{    
#ifdef HE_DEBUG
    m_IsTraversing = true;
#endif
    const_iterator it(cend());
    const_iterator end(cbegin());
    for (; it != end; --it)
        func(*(it - 1));
#ifdef HE_DEBUG
    m_IsTraversing = false;
#endif
}
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Allocator> inline
void he::List<T, Allocator>::rForEach( const std::function<void(T&)>& func )
{    
#ifdef HE_DEBUG
    m_IsTraversing = true;
#endif
    iterator it(end());
    iterator end(begin());
    for (; it != end; --it)
        func(*(it - 1));
#ifdef HE_DEBUG
    m_IsTraversing = false;
#endif
}


} //end namespace

#endif
