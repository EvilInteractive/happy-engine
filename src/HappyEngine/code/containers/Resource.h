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

#ifndef _HE_RESOURCE_H_
#define _HE_RESOURCE_H_
#pragma once

#include "ResourceFactory.h"

namespace he {

template<typename T>
class Resource
{
DECLARE_OBJECT(Resource<T>)
public:
    Resource(): m_IsLoaded(false), m_Name()
    {
    }
    virtual ~Resource()
    {
    }

    virtual void release() const
    {
        ResourceFactory<T>::getInstance()->release(m_Handle);
    }
    virtual void instantiate() const
    {
        ResourceFactory<T>::getInstance()->instantiate(m_Handle);
    }

    void setName(const he::String& name)
    {
        m_Name = name;
    }
    const he::String& getName() const
    {
        return m_Name;
    }

    virtual bool canBeGarbageCollected() const
    {
        return true;
    }

    void setLoaded()
    {
        HE_ASSERT(m_Loaded == false, "Resource %s already loaded!", m_Name.c_str());
        HE_ASSERT(m_Name.empty() == false, "Resource is loaded but does not have a name!");

        m_IsLoaded = true;
        m_LoadMutex.lock(FILE_AND_LINE);
        onLoaded(this);
        onLoaded.clear();
        m_LoadMutex.unlock();
    }
    inline bool isLoaded() const { return m_IsLoaded; }

    // Events
    void callbackOnceIfLoaded(const boost::function1<void, Resource<T>* const>& callback)
    {
        m_LoadMutex.lock(FILE_AND_LINE);
        if (m_IsLoaded)
        {
            m_LoadMutex.unlock(); //we don't know how long callback will take, and it is not necessary to keep the lock
            callback();
        }
        else
        {
            eventCallback1<void, Resource<T>* const> handler(callback);
            onLoaded += handler;
            m_LoadMutex.unlock();
        }
    }

private:
    bool m_IsLoaded;
    he::String m_Name;

    event1<void, Resource<T>* const> onLoaded;
    he::Mutex m_LoadMutex;

    // disabled assignment operator
    Resource& operator=(const Resource&);
    Resource(const Resource&);
};


template<typename T>
IMPLEMENT_OBJECT(Resource<T>)

} //end namespace

#endif
