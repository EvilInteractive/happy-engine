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

class IResource
{
public:
    typedef std::function<void(const ELoadResult result)> LoadCallback;

    virtual ~IResource() {}

    virtual void release() const = 0;
    virtual void instantiate() const = 0;

    virtual void setName(const he::String& name) = 0;
    virtual const he::String& getName() const = 0;

    virtual bool canBeGarbageCollected() const = 0;

    virtual inline bool isLoaded() const = 0;
    virtual void callbackOnceIfLoaded(const void* const id, const LoadCallback& callback) = 0;
    virtual void cancelLoadCallback(const void* const id) = 0;
    virtual void setLoaded(const ELoadResult resukt) = 0;
};

template<typename T>
class Resource : public IResource
{
DECLARE_OBJECT(Resource<T>)
public:
    Resource(): m_IsLoaded(eLoadResult_Unknown)
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

    bool isLoaded() const { return m_IsLoaded == eLoadResult_Success; }
    void callbackOnceIfLoaded(const void* const id, const LoadCallback& callback);
    void cancelLoadCallback(const void* const id);
    void setLoaded(const ELoadResult resukt);

private:
    ELoadResult m_IsLoaded;
    he::Mutex m_LoadMutex;

    he::String m_Name;
    he::ObjectList<std::pair<const void*, LoadCallback>> m_LoadCallbacks;

    // disabled assignment operator
    Resource& operator=(const Resource&);
    Resource(const Resource&);
};

template<typename T>
void he::Resource<T>::setLoaded( const ELoadResult result )
{
    m_IsLoaded = result;
    m_LoadMutex.lock(FILE_AND_LINE);
    m_LoadCallbacks.forEach([result](const std::pair<const void*, LoadCallback>& p)
    {
        p.second(result);
    });
    m_LoadCallbacks.clear();
    m_LoadCallbacks.resize(0);
    m_LoadMutex.unlock();
}

template<typename T>
void he::Resource<T>::callbackOnceIfLoaded( const void* const id, const LoadCallback& callback )
{
    m_LoadMutex.lock(FILE_AND_LINE);
    if (m_IsLoaded != eLoadResult_Unknown)
    {
        m_LoadMutex.unlock(); //we don't know how long callback will take, and it is not necessary to keep the lock
        callback(m_IsLoaded);
    }
    else
    {
        m_LoadCallbacks.add(std::make_pair(id, callback));
        m_LoadMutex.unlock();
    }
}

template<typename T>
void he::Resource<T>::cancelLoadCallback( const void* const id )
{
    m_LoadMutex.lock(FILE_AND_LINE);
    if (m_LoadCallbacks.empty() == false)
    {
        for (size_t i(0); i < m_LoadCallbacks.size();)
        {
            const std::pair<const void*, LoadCallback>& p(m_LoadCallbacks[i]);
            if (p.first == id)
            {
                m_LoadCallbacks.removeAt(i); // No break, we need to remove all instances of id
            }
            else
            {
                ++i;
            }
        }
    }
    m_LoadMutex.unlock();
}



template<typename T>
IMPLEMENT_OBJECT(Resource<T>)

} //end namespace

#endif
