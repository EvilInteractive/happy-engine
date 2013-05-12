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
//Created: 17/03/2012

#ifndef _HE_RESOURCE_FACTORY_H_
#define _HE_RESOURCE_FACTORY_H_
#pragma once

#include "ObjectFactory.h"
#include "Singleton.h"

namespace he {
   
template<typename T>
class Resource;

class IResourceFactory
{
public:
    virtual ObjectHandle create() = 0;
    virtual bool isAlive(const ObjectHandle& handle) const = 0;
    virtual void instantiate(const ObjectHandle& handle) = 0;
    virtual void release(const ObjectHandle& handle) = 0;
};

template<typename T>
class ResourceFactory : public IResourceFactory,  public ObjectFactory<T>, public Singleton<ResourceFactory<T>>
{
friend class Singleton<ResourceFactory<T>>;
template<typename R> friend class Resource;
public:

    virtual ~ResourceFactory() 
    {
        for (ObjectHandle::IndexType i(0); i < m_RefCounter.size(); ++i)
        {
            if (m_RefCounter[i] != 0)
            {
                HE_WARNING("%s: resource %s has %d references open!", this->m_DisplayName.c_str(), this->getAt(i)->getName().c_str(), m_RefCounter[i]);
            }
        }
    }
    void init(const size_t startSize, const size_t increaseSize, const he::String& displayName)
    {
        ObjectFactory<T>::init(startSize, increaseSize, displayName);
    }

    //////////////////////////////////////////////////////////////////////////
    /// ResourceFactory<T>
    //////////////////////////////////////////////////////////////////////////
    uint32 garbageCollect()
    {
        uint32 ret(0);
        for (ObjectHandle::IndexType i(0); i < m_RefCounter.size(); ++i)
        {
            if (m_RefCounter[i] == 0 && this->isAliveAt(i) && this->getAt(i)->canBeGarbageCollected())
            {
                destroyAt(i);
                ++ret;
            }
        }
        return ret;
    }

    uint32 getRefCount(const ObjectHandle handle)
    {
        return m_RefCounter[handle.getIndex()];
    }

    virtual void instantiate(const ObjectHandle handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ResourceFactory (%s): instantiating unassigned handle", this->m_DisplayName.c_str());
        HE_ASSERT(handle.getType() == this->m_Type, "ObjectHandle does not belong to this factory!");
        HE_ASSERT(ObjectFactory<T>::get(handle) != nullptr, "ResourceFactory (%s): oops handle has been garbage collected", this->m_DisplayName.c_str());
        ++m_RefCounter[handle.getIndex()];
    }

    void release(const ObjectHandle handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ResourceFactory (%s): releasing unassigned handle", this->m_DisplayName.c_str());
        HE_ASSERT(handle.getType() == this->m_Type, "ObjectHandle does not belong to this factory!");
        HE_ASSERT(m_RefCounter[handle.getIndex()] != 0, "ResourceFactory (%s): All refs are already released (%s)", this->m_DisplayName.c_str(), this->get(handle)->getName().c_str());
        --m_RefCounter[handle.getIndex()];
    }

    //////////////////////////////////////////////////////////////////////////
    /// Overrides ObjectFactory<T>
    //////////////////////////////////////////////////////////////////////////
    virtual bool isAlive(const ObjectHandle handle) const { return ObjectFactory<T>::isAlive(handle); }
    virtual ObjectHandle create()
    {
        const ObjectHandle handle(ObjectFactory<T>::create());
        instantiate(handle);
        return handle;
    }
    virtual ObjectHandle registerObject(T* const obj)
    {
        const ObjectHandle handle(ObjectFactory<T>::registerObject(obj));
        this->get(handle)->setHandle(handle);
        instantiate(handle);
        return handle;
    }

protected:
    // --> Disable
    virtual void destroyAll()
    {
        ObjectFactory<T>::destroyAll();
    }
    virtual void destroyObject(const ObjectHandle handle)
    {
        ObjectFactory<T>::destroyObject(handle);
    }
    virtual void destroyAt(const ObjectHandle::IndexType index)
    {
        ObjectFactory<T>::destroyAt(index);
    }
    // <--

    virtual void resize(const size_t newSize)
    {
        ObjectFactory<T>::resize(newSize);
        const size_t prevSize(m_RefCounter.size());
        m_RefCounter.resize(newSize);
        for (size_t i(prevSize); i < newSize; ++i)
            m_RefCounter[i] = 0;
    }
    
private:
    ResourceFactory(): ObjectFactory<T>()
    {
    }

    he::PrimitiveList<uint32> m_RefCounter;

    //Disable default copy constructor and default assignment operator
    ResourceFactory(const ResourceFactory&);
    ResourceFactory& operator=(const ResourceFactory&);
};

} //end namespace

#endif
