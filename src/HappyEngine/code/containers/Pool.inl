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

namespace he {

template<typename T>
he::Pool<T>::Pool()
    : m_InitialSize(0)
    , m_GrowSize(0)
    , m_WarningSize()
{
}

template<typename T>
he::Pool<T>::~Pool()
{

}

template<typename T>
void he::Pool<T>::init( const size_t initialSize, const size_t growSize, const size_t warningSize )
{
    m_InitialSize = initialSize;
    m_GrowSize = growSize;
    m_WarningSize = warningSize;
    grow(initialSize);
}


template<typename T>
void he::Pool<T>::destroy()
{
    HE_ASSERT(capacity() == m_FreeObjects.size(), "Pool leak detected! Pool is leaking %d objects!", capacity() - m_FreeObjects.size());
    const size_t pools(m_Pool.size());
    for (size_t i(0); i < pools; ++i)
    {
        T* pool(m_Pool[i]);
        const size_t elements(i == 0? m_InitialSize : m_GrowSize);
        for (size_t j(0); j < elements; ++j)
        {
            (pool+j)->~T();
        }
        he_free(pool);
    }
    m_FreeObjects.clear();
    m_Pool.clear();
}

template<typename T>
size_t he::Pool<T>::capacity() const
{
    if (m_Pool.size() == 0)
        return m_InitialSize;
    else
        return m_InitialSize + (m_Pool.size() - 1) * m_GrowSize;
}

template<typename T>
void he::Pool<T>::grow(const size_t amount)
{
    HE_ASSERT(m_InitialSize + m_Pool.size() * m_GrowSize < m_WarningSize, "Pool growing larger (%d) than maximum size (%d)!", m_InitialSize + m_Pool.size() * m_GrowSize, m_WarningSize);
    T* pool(static_cast<T*>(he_malloc(amount * sizeof(T))));
    for (size_t i(0); i < amount; ++i)
    {
        m_FreeObjects.add(PNEW(pool + i) T());
    }
    m_Pool.add(pool);
}

template<typename T>
T* he::Pool<T>::getFreeElement()
{
    if (m_FreeObjects.size() == 0)
        grow(m_GrowSize);
    T* obj(m_FreeObjects.back());
    m_FreeObjects.pop();
    return obj;
}

template<typename T>
void he::Pool<T>::releaseElement( T* const element )
{
    HE_ASSERT(capacity() > m_FreeObjects.size(), "Pool is releasing more objects than we have!");
    element->resetPoolElement();
    m_FreeObjects.add(element);
}

template<typename T>
void he::Pool<T>::forEach( const std::function<void(T*)>& func ) const
{
    const size_t pools(m_Pool.size());
    for (size_t i(0); i < pools; ++i)
    {
        T* const pool(m_Pool[i]);
        const size_t elements(i == 0? m_InitialSize : m_GrowSize);
        for (size_t j(0); j < elements; ++j)
        {
            func(pool + j);
        }
    }
}

} //end namespace
