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

#ifndef _HE_NETWORK_OBJECT_H_
#define _HE_NETWORK_OBJECT_H_
#pragma once

#include "NetworkObjectBase.h"
#include "NetworkObjectFactory.h"

namespace he {
namespace net {
class NetworkSerializer;
class NetworkDeserializer;

template<typename T>
class NetworkObject : public details::NetworkObjectBase
{
public:
    NetworkObject() {}
    virtual ~NetworkObject() {}

    virtual void netVisitCreate(he::net::NetworkVisitor& /*visitor*/) {}
    virtual void netVisitRemove(he::net::NetworkVisitor& /*visitor*/) {}

    virtual bool isNetSerializeDataDirty() const { return m_IsSerializeDataDirty; }
    virtual void setSerializeDataDirty(bool dirty = true) { m_IsSerializeDataDirty = dirty; }
    virtual void netSerialize(const NetworkSerializer& /*serializer*/) {}
    virtual void netDeserialize(const NetworkDeserializer& /*serializer*/) {}

    virtual void destroyReplica()
    {
        NetworkObjectFactory<T>::getInstance()->destroyObject(getHandle()); // weird but raknets way of doing it
    }
    virtual void writeObjectTypeID(net::NetworkStream* stream) const
    {
        stream->Write(NetworkObjectFactory<T>::getInstance()->getId());
    }

private:
    bool m_IsSerializeDataDirty;

    //Disable default copy constructor and default assignment operator
    NetworkObject(const NetworkObject&);
    NetworkObject& operator=(const NetworkObject&);
};

} } //end namespace

#endif
