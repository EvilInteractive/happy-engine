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

#ifndef _HE_NETWORK_OBJECT_BASE_H_
#define _HE_NETWORK_OBJECT_BASE_H_
#pragma once

#include "ReplicaManager3.h"
#include "NetworkObjectFactory.h"
#include "VariableDeltaSerializer.h"
#include "INetworkSerializable.h"


namespace he {
namespace net {
class NetworkSerializer;
class NetworkDeserializer;

namespace details {

class NetworkObjectBase : public RakNet::Replica3, public INetworkSerializable
{
public:
    //////////////////////////////////////////////////////////////////////////
    /// Replica3
    //////////////////////////////////////////////////////////////////////////
    virtual void WriteAllocationID(RakNet::Connection_RM3* /*destinationConnection*/, RakNet::BitStream* stream) const;
    virtual void DeallocReplica(RakNet::Connection_RM3* /*sourceConnection*/);

    virtual void SerializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3* /*destinationConnection*/);
    virtual bool DeserializeConstruction(RakNet::BitStream* stream, RakNet::Connection_RM3* /*sourceConnection*/);
    virtual void SerializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3* /*destinationConnection*/);
    virtual bool DeserializeDestruction(RakNet::BitStream* stream, RakNet::Connection_RM3* /*sourceConnection*/);

    virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection);
    virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection);
    virtual RakNet::RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, RakNet::ReplicaManager3 *replicaManager3);
    virtual RakNet::RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const;

    virtual RakNet::RM3SerializationResult Serialize(RakNet::SerializeParameters* serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters* deserializeParameters);

    virtual void OnUserReplicaPreSerializeTick(void);
    virtual void OnPoppedConnection(RakNet::Connection_RM3* droppedConnection);

    ////////////////////////////////////////////////////////////////////////// 

    //////////////////////////////////////////////////////////////////////////
    /// INetworkSerializable
    //////////////////////////////////////////////////////////////////////////
    virtual void serializeCreate(net::NetworkStream* stream) const = 0;
    virtual bool deserializeCreate(net::NetworkStream* stream) = 0;
    virtual void serializeRemove(net::NetworkStream* stream) const = 0;
    virtual bool deserializeRemove(net::NetworkStream* stream) = 0;

    virtual bool isSerializeDataDirty() const = 0;
    virtual void serialize(const NetworkSerializer& serializer) = 0;
    virtual void deserialize(const NetworkDeserializer& serializer) = 0;
    //////////////////////////////////////////////////////////////////////////

    virtual void destroyReplica() = 0;
    virtual void writeObjectTypeID(net::NetworkStream* stream) const = 0;

    //////////////////////////////////////////////////////////////////////////

    virtual void setHandle(const ObjectHandle& handle) { m_Handle = handle; }
    virtual const ObjectHandle& getHandle() const { return m_Handle; }

    // Call this just after creation, synced in serializeCreate!
    virtual void setOwner(const NetworkID& id) { creatingSystemGUID = id; }
    virtual NetworkID getOwner() { return GetCreatingSystemGUID(); }
    
    virtual ~NetworkObjectBase() {}
protected:
    NetworkObjectBase(): m_Serialize(false) {}

private:
    RakNet::VariableDeltaSerializer m_VariableDeltaSerializer;
    ObjectHandle m_Handle;
    bool m_Serialize;
};

} } } //end namespace

#endif
