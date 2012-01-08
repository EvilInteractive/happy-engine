//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 27/11/2011

#ifndef _HE_SLOT_P_CONTAINER_H_
#define _HE_SLOT_P_CONTAINER_H_
#pragma once

#include <vector>
#include <queue>
#include "HappyTypes.h"
#include "HeAssert.h"

namespace he {

template<typename T>
class SlotPContainer
{
public:
    SlotPContainer() {}
    ~SlotPContainer() {}
    //default copy OK

    uint insert(const T& obj)
    {
        if (m_FreeSlots.size() > 0)
        {
            uint id(m_FreeSlots.front());
            m_FreeSlots.pop();
            m_MemPool[id] = obj;
            return id;
        }
        else
        {
            uint id(m_MemPool.size());
            m_MemPool.push_back(obj);
            return id;
        }
    }
    const T& get(uint id)
    {
        ASSERT(id < m_MemPool.size(), "id is not in a valid range");
        ASSERT(m_MemPool[id] != nullptr, "no object @id");
        return m_MemPool[id];
    }
    T remove(uint id)
    {
        ASSERT(id < m_MemPool.size(), "id is not in a valid range");

        m_FreeSlots.push(id);

        T& pObj(m_MemPool[id]);
        m_MemPool[id] = nullptr;

        return pObj;
    }
    uint getId(const T& obj)
    {
        ASSERT(obj != nullptr, "object == nullptr");

        for (uint i(0); i < m_MemPool.size(); ++i)
            if (m_MemPool[i] == obj)
                return i;
        ASSERT(false, "object not present in container");
        return 0;
    }

    T& operator[](uint id)
    {
        ASSERT(id < m_MemPool.size(), "id is not in a valid range");
        ASSERT(m_MemPool[id] != nullptr, "no object @id");
        return m_MemPool[id];
    }
    const T& operator[](uint id) const
    {
        ASSERT(id < m_MemPool.size(), "id is not in a valid range");
        ASSERT(m_MemPool[id] != nullptr, "no object @id");
        return m_MemPool[id];
    }

    template<typename R>
    class const_iterator
    {
    public: typedef typename std::vector<R>::const_iterator _iterator;
    private:
        _iterator m_It;
        _iterator m_EndIt;

    public:
        const_iterator(const _iterator& iterator, const _iterator& end):
          m_It(iterator), m_EndIt(end) { }

        const_iterator& operator++()
        {
            do
            {
                ++m_It;
            } while (m_It != m_EndIt && (*m_It) == nullptr);

            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator ret(*this);
            ++ret;
            return ret;
        }
        bool operator==(const const_iterator& it)
        {
            return (m_It == it.m_It);
        }
        bool operator!=(const const_iterator& it)
        {
            return (m_It != it.m_It);
        }
        const R& operator*()
        {
            return m_It.operator*();
        }
        const R*& operator->()
        {
            return m_It.operator->();
        }

    };

    const_iterator<T> cbegin()
    {
        return const_iterator<T>(m_MemPool.cbegin(), m_MemPool.cend());
    }
    const_iterator<T> cend()
    {
        return const_iterator<T>(m_MemPool.cend(), m_MemPool.cend());
    }

    uint size()
    {
        return m_MemPool.size() - m_FreeSlots.size();
    }

private:
    
    std::vector<T> m_MemPool;
    std::queue<uint> m_FreeSlots;

    //disable default assignment operator
    SlotPContainer& operator=(const SlotPContainer&);
};


template<class _InIt, class _Fn1> inline
_Fn1 for_each(_InIt _First, _InIt _Last, _Fn1 _Func)
{	// perform function for each element
    for (; _First != _Last; ++_First)
        _Func(*_First);
    return (_Func);
}

} //end namespace

#endif
