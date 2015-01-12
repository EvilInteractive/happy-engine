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
//Created: 17/03/2012

#ifndef _HE_OBJECT_FACTORY_H_
#define _HE_OBJECT_FACTORY_H_
#pragma once
#pragma message("-- Including object factory --")

#include "Singleton.h"

namespace he {

#define DECLARE_OBJECT(Type) \
private: \
    he::ObjectHandle m_Handle; \
public: \
    static he::ObjectHandle::ObjectType s_ObjectType; \
    inline const he::ObjectHandle& getHandle() const { return m_Handle; } \
    inline void setHandle(const he::ObjectHandle& handle) { m_Handle = handle; }
#define IMPLEMENT_OBJECT(Type) \
    he::ObjectHandle::ObjectType Type::s_ObjectType = he::ObjectHandle::s_UnassignedType;

namespace details {
class SINGLETON(ObjectFactoryTypeManager)
{
    friend class Singleton<ObjectFactoryTypeManager>;
    ObjectFactoryTypeManager(): m_LastType(0) {}
    ~ObjectFactoryTypeManager() {}
public:
    template<typename T>
    ObjectHandle::ObjectType getObjectType()
    {
        m_Mutex.lock(FILE_AND_LINE);
        if (T::s_ObjectType != ObjectHandle::s_UnassignedType)
        {
            m_Mutex.unlock();
            return T::s_ObjectType;
        }
        else
        {
            T::s_ObjectType = ++m_LastType;
            m_Mutex.unlock();
            return T::s_ObjectType;
        }
    }

private:
    ObjectHandle::ObjectType m_LastType;
    he::Mutex m_Mutex;
};
}

template<typename T>
class DefaultObjectAllocator
{
public:
    static T* allocate() { return NEW T(); }
    static void deallocate(T* const obj) { delete obj; }
};

template<typename T>
class NoCreateObjectAllocator
{
public:
    static T* allocate() { LOG(LogType_ProgrammerAssert, "Should not be called!"); return nullptr; }
    static void deallocate(T* const obj) { delete obj; }
};

template<typename T, typename Allocator=DefaultObjectAllocator<T>>
class ObjectFactory
{
public:
    ObjectFactory(): m_IncreaseSize(32), m_Type(0), m_DisplayName("unknown")
    {
    }

    void init(const size_t startSize, const size_t increaseSize, const he::String& displayName)
    {
        m_IncreaseSize = increaseSize;
        m_DisplayName = displayName;
        m_Type = details::ObjectFactoryTypeManager::getInstance()->getObjectType<T>();
        resize(startSize);
    }

    virtual ~ObjectFactory()
    {
#ifdef HE_DEBUG
        size_t leakCounter(0);
        for (ObjectHandle::IndexType i(0); i < m_Pool.size(); ++i)
        {
            if (isAliveAt(i))
            {
                ++leakCounter;
            }
        }
        if (leakCounter > 0)
            HE_WARNING("%s: %d leaks detected!", m_DisplayName.c_str(), leakCounter);
#endif
    }
    virtual void destroyAll()
    {
        const size_t poolSize(m_Pool.size());
        for (ObjectHandle::IndexType i(0); i < poolSize; ++i)
        {
            if (m_Pool[i] != nullptr)
            {
                destroyAt(i);
            }
        }
    }
    virtual ObjectHandle create()
    {
        const ObjectHandle handle(getFreeHandle());
        T* const obj(Allocator::allocate());
        HE_ASSERT(obj != nullptr, "Object allocator returned nullptr! (%s)", m_DisplayName.c_str());
        obj->setHandle(handle);
        m_Pool[handle.getIndex()] = obj;
        return handle;
    }
    virtual ObjectHandle registerObject(T* const obj)
    {
        const ObjectHandle handle(getFreeHandle());
        m_Pool[handle.getIndex()] = obj;
        obj->setHandle(handle);
        return handle;
    }
    virtual void unregisterObject(const ObjectHandle handle)
    {
       HE_ASSERT(handle != ObjectHandle::unassigned, "ObjectFactory (%s): unregistering unassigned handle", m_DisplayName.c_str());
       HE_ASSERT(handle.getType() == m_Type, "ObjectHandle does not belong to this factory!");
       if (m_Salt[handle.getIndex()] != handle.getSalt())
       {
           HE_ERROR("ObjectFactory (%s): salt mismatch when unregistering object", m_DisplayName.c_str());
       }
       else
       {
           unregisterAt(handle.getIndex());
       }
    }
    virtual void destroyObject(const ObjectHandle handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ObjectFactory (%s): destroying unassigned handle", m_DisplayName.c_str());
        HE_ASSERT(handle.getType() == m_Type, "ObjectHandle does not belong to this factory!");
        if (m_Salt[handle.getIndex()] != handle.getSalt())
        {
            HE_ERROR("ObjectFactory (%s): salt mismatch when destroying object", m_DisplayName.c_str());
        }
        else
        {
            destroyAt(handle.getIndex());
        }
    }
   virtual void unregisterAt(const ObjectHandle::IndexType index)
   {
       HE_IF_ASSERT(m_Pool[index] != nullptr, "ObjectFactory (%s): unregistering non existing handle", m_DisplayName.c_str())
       {
           m_Pool[index] = nullptr;
           ++m_Salt[index];
           m_FreeHandles.push(index);
       }
       HE_ASSERT(m_Salt[index] + 1 < ObjectHandle::s_MaxSalts, "ObjectFactory (%s): salt is growing out of bounds", m_DisplayName.c_str());
   }
    virtual void destroyAt(const ObjectHandle::IndexType index)
    {
        HE_IF_ASSERT(m_Pool[index] != nullptr, "ObjectFactory (%s): destroying non existing handle", m_DisplayName.c_str())
        {
            Allocator::deallocate(m_Pool[index]);
            m_Pool[index] = nullptr;
            ++m_Salt[index];
            m_FreeHandles.push(index);
        }
        HE_ASSERT(m_Salt[index] + 1 < ObjectHandle::s_MaxSalts, "ObjectFactory (%s): salt is growing out of bounds", m_DisplayName.c_str());
    }

    virtual T* get(const ObjectHandle handle) const
    {
        if (handle != ObjectHandle::unassigned)
        {
            HE_ASSERT(handle.getType() == m_Type, "ObjectHandle does not belong to this factory!");
            HE_ASSERT(m_Salt[handle.getIndex()] == handle.getSalt(), "ObjectFactory (%s): salt mismatch when getting object", m_DisplayName.c_str());
            return m_Pool[handle.getIndex()];
        }
        else
        {
            return nullptr;
        }
    }
    virtual T* getAt(const ObjectHandle::IndexType index) const
    {
        return m_Pool[index];
    }

    virtual bool isAlive(const ObjectHandle handle) const
    {
        return m_Salt[handle.getIndex()] == handle.getSalt();
    }
    virtual bool isAliveAt(const size_t index) const
    {
        return m_Pool[index] != nullptr;
    }
    bool isEmpty() const { return m_FreeHandles.size() == m_Pool.size(); }
    size_t getSize() const { return m_Pool.size() - m_FreeHandles.size(); }

protected:
    virtual void resize(const size_t newSize)
    {
        HE_ASSERT(newSize < ObjectHandle::s_MaxIndices, "ObjectFactory (%s): resize out of range: %d", m_DisplayName.c_str(), static_cast<int>(newSize));
        HE_CONDITIONAL_WARNING(m_Pool.size() != 0, "ObjectFactory (%s): increasing pool to %d", m_DisplayName.c_str(), static_cast<int>(newSize));
        const ObjectHandle::IndexType oldSize(static_cast<ObjectHandle::IndexType>(m_Pool.size()));
        m_Pool.resize(newSize);
        m_Salt.resize(newSize);
        for (ObjectHandle::IndexType i(oldSize); i < newSize; ++i)
        {
            m_FreeHandles.push(i);
            m_Pool[i] = nullptr;
            m_Salt[i] = 0;
        }
    }

    he::String m_DisplayName;

protected:
    ObjectHandle::ObjectType m_Type;

private:
    ObjectHandle getFreeHandle()
    {
        if (m_FreeHandles.size() == 0)
        {
            resize(m_Pool.size() + m_IncreaseSize);
        }

        const ObjectHandle::IndexType index(m_FreeHandles.front());
        m_FreeHandles.pop();
        const ObjectHandle handle(m_Type, m_Salt[index], index);
        return handle;
    }

    size_t m_IncreaseSize;

    he::PrimitiveList<T*> m_Pool;
    he::PrimitiveList<ObjectHandle::SaltType> m_Salt;
    std::queue<ObjectHandle::IndexType> m_FreeHandles;

    //Disable default copy constructor and default assignment operator
    ObjectFactory(const ObjectFactory&);
    ObjectFactory& operator=(const ObjectFactory&);
};

} //end namespace

#endif
