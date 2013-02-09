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

#ifndef _HE_FIXEDSIZE_LIST_H_
#define _HE_FIXEDSIZE_LIST_H_
#pragma once

#include "Creator.h"

namespace he {
    
template<typename T, size_t SIZE>
class FixedSizeList
{
public:
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef boost::function2<int, const T&, const T&> Sorter;
    typedef boost::function2<bool, const T&, const T&> Comparer;
    typedef boost::function1<bool, const T&> Pred;

    explicit FixedSizeList();
    ~FixedSizeList();

    inline bool contains(const T& element) const; // O(n)
    inline bool find(const T& element, size_t& outIndex) const; // O(n)
    inline bool find_if(const Pred& pred, size_t& outIndex) const; // O(n)
    inline bool rfind_if(const Pred& pred, size_t& outIndex) const; // O(n)
    inline bool rfind(const T& element, size_t& outIndex) const; // O(n)

    inline size_t size() const { return SIZE; }

    inline const T& back() const;
    inline const T& front() const;
    inline T& back();
    inline T& front();

    inline T& operator[](const size_t index);
    inline const T& operator[](const size_t index) const;

    inline iterator begin() { return m_Buffer; }
    inline iterator end() { return m_Buffer + SIZE; }
    inline const_iterator cbegin() const { return m_Buffer; }
    inline const_iterator cend() const { return m_Buffer + SIZE; }

    inline void forEach(const boost::function1<void, const T&>& func) const;
    inline void forEach(const boost::function1<void, T&>& func);
    inline void rForEach(const boost::function1<void, const T&>& func) const;
    inline void rForEach(const boost::function1<void, T&>& func);
        
private:    
    T m_Buffer[SIZE];

    //Disable default copy constructor and default assignment operator
    FixedSizeList(const FixedSizeList&);
    FixedSizeList& operator=(const FixedSizeList&);
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template<typename T, size_t SIZE> inline
he::FixedSizeList<T, SIZE>::FixedSizeList()
{
}

template<typename T, size_t SIZE> inline
he::FixedSizeList<T, SIZE>::~FixedSizeList()
{
}

//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE>
bool he::FixedSizeList<T, SIZE>::contains( const T& element ) const
{
    size_t i(0);
    return find(element, i);
}
template<typename T, size_t SIZE> inline
bool he::FixedSizeList<T, SIZE>::find( const T& element, size_t& outIndex ) const
{
    bool result(false);
    for (size_t i(0); i < SIZE; ++i)
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
template<typename T, size_t SIZE> inline
bool he::FixedSizeList<T, SIZE>::find_if( const Pred& condition, size_t& outIndex ) const
{
    bool result(false);
    for (size_t i(0); i < SIZE; ++i)
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
template<typename T, size_t SIZE> inline
bool he::FixedSizeList<T, SIZE>::rfind_if( const Pred& condition, size_t& outIndex ) const
{
    bool result(false);
    for (int i(SIZE - 1); i >= 0; --i)
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
template<typename T, size_t SIZE> inline
bool he::FixedSizeList<T, SIZE>::rfind( const T& element, size_t& outIndex ) const
{
    for (int i(SIZE - 1); i >= 0; --i)
    {
        if (m_Buffer[i] == element)
        {
            outIndex = i;
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
const T& he::FixedSizeList<T, SIZE>::front() const
{
    HE_ASSERT(SIZE > 0, "No elements in FixedSizeList! Index out of bounds exception");
    return m_Buffer[0];
}

template<typename T, size_t SIZE> inline
const T& he::FixedSizeList<T, SIZE>::back() const
{
    HE_ASSERT(SIZE > 0, "No elements in FixedSizeList! Index out of bounds exception");
    return m_Buffer[SIZE - 1];
}
template<typename T, size_t SIZE>
T& he::FixedSizeList<T, SIZE>::front()
{
    HE_ASSERT(SIZE > 0, "No elements in FixedSizeList! Index out of bounds exception");
    return m_Buffer[0];
}
template<typename T, size_t SIZE>
T& he::FixedSizeList<T, SIZE>::back()
{
    HE_ASSERT(SIZE > 0, "No elements in FixedSizeList! Index out of bounds exception");
    return m_Buffer[SIZE - 1];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
T& he::FixedSizeList<T, SIZE>::operator[]( const size_t index )
{
    HE_ASSERT(index < SIZE, "Index out of bounds! getting %d from #%d elements", index, SIZE);
    return m_Buffer[index];
}

template<typename T, size_t SIZE> inline
const T& he::FixedSizeList<T, SIZE>::operator[]( const size_t index ) const
{
    HE_ASSERT(index < SIZE, "Index out of bounds! getting %d from #%d elements", index, SIZE);
    return m_Buffer[index];
}
//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
void he::FixedSizeList<T, SIZE>::forEach( const boost::function1<void, const T&>& func ) const
{    
    for (size_t i(0); i < SIZE; ++i)
        func(m_Buffer[i]);
}
//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
void he::FixedSizeList<T, SIZE>::forEach( const boost::function1<void, T&>& func )
{    
    for (size_t i(0); i < SIZE; ++i)
        func(m_Buffer[i]);
}
//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
void he::FixedSizeList<T, SIZE>::rForEach( const boost::function1<void, const T&>& func ) const
{    
    for (size_t i(0); i < SIZE; ++i)
        func(m_Buffer[i - 1]);
}
//////////////////////////////////////////////////////////////////////////
template<typename T, size_t SIZE> inline
void he::FixedSizeList<T, SIZE>::rForEach( const boost::function1<void, T&>& func )
{    
    for (size_t i(0); i < SIZE; ++i)
        func(m_Buffer[i - 1]);
}


} //end namespace

#endif
