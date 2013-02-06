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
class ResourceFactory : public IResourceFactory,  public ObjectFactory<T>
{
template<typename R> friend class Resource;
public:

    //////////////////////////////////////////////////////////////////////////
    ///  Singleton
    //////////////////////////////////////////////////////////////////////////
    static void init(size_t startSize, size_t increaseSize, const std::string& displayName)
    {
        HE_ASSERT(s_Instance == nullptr, "initing an already inited resource factory: %s", displayName.c_str());
        s_Instance = NEW ResourceFactory<T>(startSize, increaseSize, displayName);
    }
    static void destroy()
    {
        delete s_Instance;
    }
    static ResourceFactory<T>* getInstance()
    {
        HE_ASSERT(s_Instance != nullptr, "Resource factory has not been initialized!");
        return s_Instance;
    }

    //////////////////////////////////////////////////////////////////////////
    /// ResourceFactory<T>
    //////////////////////////////////////////////////////////////////////////
    void garbageCollect()
    {
        for (ObjectHandle::IndexType i(0); i < m_RefCounter.size(); ++i)
        {
            if (m_RefCounter[i] == 0 && this->isAliveAt(i) && this->getAt(i)->canBeGarbageCollected())
            {
                destroyAt(i);
            }
        }
    }

    uint32 getRefCount(const ObjectHandle& handle)
    {
        return m_RefCounter[handle.index];
    }

    virtual void instantiate(const ObjectHandle& handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ResourceFactory (%s): instantiating unassigned handle", this->m_DisplayName.c_str());
        HE_ASSERT(handle.type == this->m_Type, "ObjectHandle does not belong to this factory!");
        HE_ASSERT(ObjectFactory<T>::get(handle) != nullptr, "ResourceFactory (%s): oops handle has been garbage collected", this->m_DisplayName.c_str());
        ++m_RefCounter[handle.index];
    }

    void release(const ObjectHandle& handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ResourceFactory (%s): releasing unassigned handle", this->m_DisplayName.c_str());
        HE_ASSERT(handle.type == this->m_Type, "ObjectHandle does not belong to this factory!");
        HE_ASSERT(m_RefCounter[handle.index] != 0, "ResourceFactory (%s): All refs are already released (%s)", this->m_DisplayName.c_str(), this->get(handle)->getName().c_str());
        --m_RefCounter[handle.index];
    }

    //////////////////////////////////////////////////////////////////////////
    /// Overrides ObjectFactory<T>
    //////////////////////////////////////////////////////////////////////////
    virtual bool isAlive(const ObjectHandle& handle) const { return ObjectFactory<T>::isAlive(handle); }
    virtual ObjectHandle create()
    {
        ObjectHandle handle(ObjectFactory<T>::create());
        instantiate(handle);
        return handle;
    }
    virtual ObjectHandle registerObject(T* obj)
    {
        ObjectHandle handle(ObjectFactory<T>::registerObject(obj));
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
    virtual void destroyObject(const ObjectHandle& handle)
    {
        ObjectFactory<T>::destroyObject(handle);
    }
    virtual void destroyAt(ObjectHandle::IndexType index)
    {
        ObjectFactory<T>::destroyAt(index);
    }
    // <--

    virtual void resize(size_t newSize)
    {
        ObjectFactory<T>::resize(newSize);
        size_t prevSize(m_RefCounter.size());
        m_RefCounter.resize(newSize);
        for (size_t i(prevSize); i < newSize; ++i)
            m_RefCounter[i] = 0;
    }
    
private:
    static ResourceFactory<T>* s_Instance;

    ResourceFactory(size_t startSize, size_t increaseSize, const std::string& displayName): ObjectFactory<T>()
    {
        ObjectFactory<T>::init(startSize, increaseSize, displayName);
    }
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

    he::PrimitiveList<uint32> m_RefCounter;

    //Disable default copy constructor and default assignment operator
    ResourceFactory(const ResourceFactory&);
    ResourceFactory& operator=(const ResourceFactory&);
};

template<typename T>
ResourceFactory<T>* ResourceFactory<T>::s_Instance = nullptr;

} //end namespace

#endif
