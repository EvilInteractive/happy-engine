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
//Created: 2013/10/23

#ifndef _HE_Pool_H_
#define _HE_Pool_H_
#pragma once

namespace he {

class PoolElement
{
    virtual ~PoolElement() {} 
    virtual void resetPoolElement() = 0;
};

template<typename T>
class Pool
{
public:
    Pool();
    ~Pool();

    inline void init(const size_t initialSize, const size_t growSize, const size_t warningSize);
    inline void destroy();

    inline T* getFreeElement();
    inline void releaseElement(T* const element);

    size_t capacity() const;

    inline void forEach(const boost::function1<void, T*>& func) const; // all items in pool, also the items in the free list

private:
    void grow(const size_t amount);

    size_t m_InitialSize;
    size_t m_GrowSize;
    size_t m_WarningSize;

    he::PrimitiveList<T*> m_FreeObjects;
    he::PrimitiveList<T*> m_Pool;

    //Disable default copy constructor and default assignment operator
    Pool(const Pool&);
    Pool& operator=(const Pool&);
};

} //end namespace

#include "Pool.inl"

#endif
