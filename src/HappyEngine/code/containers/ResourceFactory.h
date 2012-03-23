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
class ResourceFactory : public ObjectFactory<T>
{
public:

    //////////////////////////////////////////////////////////////////////////
    ///  Singleton
    //////////////////////////////////////////////////////////////////////////
    static void init(uint startSize, uint increaseSize, const std::string& displayName)
    {
        HE_ASSERT(s_Instance == nullptr, "initing an already inited resource factory: " + displayName);
        s_Instance = NEW ResourceFactory<T>(startSize, increaseSize, displayName);
    }
    static void destroy()
    {
        delete s_Instance;
    }
    static ResourceFactory<T>* getInstance()
    {
        ASSERT(s_Instance != nullptr, "Resource factory has not been initialized!");
        return s_Instance;
    }

    //////////////////////////////////////////////////////////////////////////
    /// ResourceFactory<T>
    //////////////////////////////////////////////////////////////////////////
    void garbageCollect()
    {
        for (uint i(0); i < m_RefCounter.size(); ++i)
        {
            if (m_RefCounter[i] == 0)
            {
                destroyObject(i);
            }
        }
    }

    void instantiate(const ObjectHandle& handle)
    {
        HE_ASSERT(ObjectFactory<T>::get(handle) != nullptr, "ResourceFactory (" + m_DisplayName + "): oops handle has been garbage collected");
        ++m_RefCounter[handle];
    }
    void release(const ObjectHandle& handle)
    {
        HE_ASSERT(m_RefCounter[handle] != 0, "ResourceFactory (" + m_DisplayName + "): All refs are already released");
        --m_RefCounter[handle];
    }

    uint getRefCount(const ObjectHandle& handle)
    {
        return m_RefCounter[handle.index];
    }

    //////////////////////////////////////////////////////////////////////////
    /// Overrides ObjectFactory<T>
    //////////////////////////////////////////////////////////////////////////
    virtual ObjectHandle create()
    {
        ObjectHandle handle(ObjectFactory<T>::create());
        get(handle)->setHandle(handle);
        return handle;
    }
    virtual ObjectHandle register(T* obj)
    {
        ObjectHandle handle(ObjectFactory<T>::register(obj));
        get(handle)->setHandle(handle);
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
    // <--

    virtual void resize(uint newSize)
    {
        ObjectFactory<T>::resize(newSize);
        uint prevSize(m_RefCounter.size());
        m_RefCounter.resize(newSize);
        for (uint i(prevSize); i < newsize; ++i)
            m_RefCounter[i] = 0;
    }
    
private:
    static ResourceFactory<T> s_Instance;

    ResourceFactory(uint startSize, uint increaseSize, const std::string& displayName):
       ObjectFactory<T>(startSize, increaseSize, displayName)
    {
    }
    virtual ~ResourceFactory() 
    {
    }

    std::vector<uint> m_RefCounter;

    //Disable default copy constructor and default assignment operator
    ResourceFactory(const ResourceFactory&);
    ResourceFactory& operator=(const ResourceFactory&);
};

} //end namespace

#endif
