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
//Created: 23/06/2012

#ifndef _HE_NETWORK_OBJECT_FACTORY_H_
#define _HE_NETWORK_OBJECT_FACTORY_H_
#pragma once

#include "ObjectFactory.h"
#include "Singleton.h"
#include "NetworkManager.h"

namespace he {
namespace net {
namespace details {
    class NetworkObjectBase;
}
class INetworkObjectFactory
{
public:
    virtual ~INetworkObjectFactory() {}

    virtual const NetworkObjectTypeID& getId() const = 0;
    virtual void setId(const NetworkObjectTypeID& id) = 0;

    // internal
    virtual details::NetworkObjectBase* createReplica() = 0;
};

// T inherits NetworkObjectBase
template<typename T>
class NetworkObjectFactory : public ObjectFactory<T>, public INetworkObjectFactory, public Singleton<NetworkObjectFactory<T>>
{
friend class Singleton<NetworkObjectFactory<T>>;
public:
    virtual ~NetworkObjectFactory() {}
    
    // TODO: should not be public, this class should always be inherited
    void init(size_t startSize, size_t increaseSize, const he::String& displayName)
    {
        ObjectFactory<T>::init(startSize, increaseSize, displayName);
    }

    //////////////////////////////////////////////////////////////////////////
    // ObjectFactory
    //////////////////////////////////////////////////////////////////////////
    virtual ObjectHandle create()
    {
        ObjectHandle handle(ObjectFactory<T>::create());
        details::NetworkObjectBase* obj(this->get(handle));
        obj->setHandle(handle);
        NetworkManager* const man(NETWORK);
        obj->setNetworkOwner(man->getNetworkId());
        man->Reference(obj);
        return handle;
    }
    virtual ObjectHandle registerObject(T* obj)
    {
        ObjectHandle handle(ObjectFactory<T>::registerObject(obj));
        obj->setHandle(handle);
        NetworkManager* const man(NETWORK);
        obj->setNetworkOwner(man->getNetworkId());
        man->Reference(obj);
        return handle;
    }
    virtual void destroyObject(const ObjectHandle& handle)
    {
        NetworkManager* const man(NETWORK);
        if (man->isConnected())
            man->BroadcastDestruction(this->get(handle), RakNet::UNASSIGNED_SYSTEM_ADDRESS);
        ObjectFactory<T>::destroyObject(handle);
    }
    virtual void destroyAt(ObjectHandle::IndexType index)
    {
        NetworkManager* const man(NETWORK);
        if (man->isConnected())
            man->BroadcastDestruction(this->getAt(index), RakNet::UNASSIGNED_SYSTEM_ADDRESS);
        ObjectFactory<T>::destroyAt(index);
    }
    //////////////////////////////////////////////////////////////////////////

    virtual const NetworkObjectTypeID& getId() const { return m_Id; }
    virtual void setId(const NetworkObjectTypeID& id) { m_Id = id; }

    virtual details::NetworkObjectBase* createReplica() 
    { 
        ObjectHandle handle(ObjectFactory<T>::create());
        return this->get(handle); 
    };

protected: 
    NetworkObjectFactory()
    {
        NETWORK->registerFactory(this);
    }

private:
    NetworkObjectTypeID m_Id;

    //Disable default copy constructor and default assignment operator
    NetworkObjectFactory(const NetworkObjectFactory&);
    NetworkObjectFactory& operator=(const NetworkObjectFactory&);
};

} } //end namespace

#endif
