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
//Created: 23/06/2012

#ifndef _HE_NETWORK_OBJECT_FACTORY_H_
#define _HE_NETWORK_OBJECT_FACTORY_H_
#pragma once

#include "ObjectFactory.h"
#include "Singleton.h"
#include "NetworkManager.h"
#include "NetworkObjectBase.h"

namespace he {
namespace net {
namespace details {
class NetworkObjectBase;
}

class INetworkObjectFactory
{
public:
    virtual ~INetworkObjectFactory() {}

    virtual const NetworkObjectID& getId() const = 0;
    virtual void setId(const NetworkObjectID& id) = 0;

    // internal
    virtual details::NetworkObjectBase* createReplica() = 0;
};

// T inherits NetworkObjectBase
template<typename T>
class NetworkObjectFactory : public ObjectFactory<T>, public INetworkObjectFactory, public Singleton<NetworkObjectFactory<T>>
{
public:
    NetworkObjectFactory()
    {
        NETWORK->registerFactory(this);
    }
    virtual ~NetworkObjectFactory() {}

    //////////////////////////////////////////////////////////////////////////
    // ObjectFactory
    //////////////////////////////////////////////////////////////////////////
    virtual ObjectHandle create()
    {
        ObjectHandle handle(ObjectFactory<T>::create());
        details::NetworkObjectBase* obj(get(handle));
        obj->setHandle(handle);
        obj->setOwner(NETWORK->getNetworkId());
        NETWORK->Reference(obj);
        return handle;
    }
    virtual ObjectHandle registerObject(T* obj)
    {
        ObjectHandle handle(ObjectFactory<T>::registerObject(obj));
        obj->setHandle(handle);
        obj->setOwner(NETWORK->getNetworkId());
        NETWORK->Reference(obj);
        return handle;
    }
    virtual void destroyObject(const ObjectHandle& handle)
    {
        if (NETWORK->isConnected())
            NETWORK->BroadcastDestruction(get(handle), RakNet::UNASSIGNED_SYSTEM_ADDRESS);
        ObjectFactory<T>::destroyObject(handle);
    }
    virtual void destroyAt(ObjectHandle::IndexType index)
    {
        if (NETWORK->isConnected())
            NETWORK->BroadcastDestruction(getAt(index), RakNet::UNASSIGNED_SYSTEM_ADDRESS);
        ObjectFactory<T>::destroyAt(index);
    }
    //////////////////////////////////////////////////////////////////////////

    virtual const NetworkObjectID& getId() const { return m_Id; }
    virtual void setId(const NetworkObjectID& id) { m_Id = id; }

    virtual details::NetworkObjectBase* createReplica() 
    { 
        ObjectHandle handle(ObjectFactory<T>::create());
        details::NetworkObjectBase* obj(get(handle));
        obj->setHandle(handle);
        return get(handle); 
    };
    
private:
    NetworkObjectID m_Id;

    //Disable default copy constructor and default assignment operator
    NetworkObjectFactory(const NetworkObjectFactory&);
    NetworkObjectFactory& operator=(const NetworkObjectFactory&);
};

} } //end namespace

#endif
