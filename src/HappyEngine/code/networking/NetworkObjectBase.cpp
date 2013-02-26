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
#include "HappyPCH.h" 

#include "NetworkObjectBase.h"
#include "GetTime.h"
#include "ReplicaManager3.h"
#include "NetworkSerializer.h"
#include "NetworkManager.h"
#include "NetworkVisitor.h"

namespace he {
namespace net {
namespace details {

void NetworkObjectBase::WriteAllocationID( RakNet::Connection_RM3* /*destinationConnection*/, RakNet::BitStream* stream ) const
{
    writeObjectTypeID(stream);
}

void NetworkObjectBase::DeallocReplica( RakNet::Connection_RM3* /*sourceConnection*/ )
{
    destroyReplica();
}

void NetworkObjectBase::SerializeConstruction( RakNet::BitStream* stream, RakNet::Connection_RM3* destinationConnection )
{
    m_VariableDeltaSerializer.AddRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
    NetworkVisitor visitor(stream, NetworkVisitor::eOpenType_Write);
    netVisitCreate(visitor);
}

bool NetworkObjectBase::DeserializeConstruction( RakNet::BitStream* stream, RakNet::Connection_RM3* /*sourceConnection*/ )
{
    NetworkVisitor visitor(stream, NetworkVisitor::eOpenType_Read);
    netVisitCreate(visitor);
    return true;
}

void NetworkObjectBase::SerializeDestruction( RakNet::BitStream* stream, RakNet::Connection_RM3* destinationConnection )
{
    m_VariableDeltaSerializer.RemoveRemoteSystemVariableHistory(destinationConnection->GetRakNetGUID());
    NetworkVisitor visitor(stream, NetworkVisitor::eOpenType_Write);
    netVisitRemove(visitor);
}

bool NetworkObjectBase::DeserializeDestruction( RakNet::BitStream* stream, RakNet::Connection_RM3* /*sourceConnection*/ )
{
    NetworkVisitor visitor(stream, NetworkVisitor::eOpenType_Read);
    netVisitRemove(visitor);
    return true;
}

RakNet::RM3QuerySerializationResult NetworkObjectBase::QuerySerialization( RakNet::Connection_RM3* destinationConnection )
{
    RakNet::RM3QuerySerializationResult result(QuerySerialization_ClientSerializable(destinationConnection, NETWORK->IsHost()));
    if (result == RakNet::RM3QSR_CALL_SERIALIZE)
    {
        if (m_Serialize)
            result = RakNet::RM3QSR_CALL_SERIALIZE;
        else
            result = RakNet::RM3QSR_DO_NOT_CALL_SERIALIZE;
    }
    return result;
}

bool NetworkObjectBase::QueryRemoteConstruction( RakNet::Connection_RM3* sourceConnection )
{
    return QueryRemoteConstruction_ServerConstruction(sourceConnection, NETWORK->IsHost());
}

RakNet::RM3ConstructionState NetworkObjectBase::QueryConstruction( RakNet::Connection_RM3* destinationConnection, RakNet::ReplicaManager3* /*replicaManager3*/ )
{
    return QueryConstruction_ServerConstruction(destinationConnection, NETWORK->IsHost());
}

RakNet::RM3ActionOnPopConnection NetworkObjectBase::QueryActionOnPopConnection( RakNet::Connection_RM3* droppedConnection ) const
{
    return QueryActionOnPopConnection_Server(droppedConnection);
}

RakNet::RM3SerializationResult NetworkObjectBase::Serialize( RakNet::SerializeParameters* serializeParameters )
{
    RakNet::VariableDeltaSerializer::SerializationContext context;

    serializeParameters->pro[0].reliability = RELIABLE_SEQUENCED;
    serializeParameters->messageTimestamp = RakNet::GetTime();

    m_VariableDeltaSerializer.BeginIdenticalSerialize(
        &context, 
        serializeParameters->whenLastSerialized == 0, 
        &serializeParameters->outputBitstream[0]);
    {
        NetworkSerializer serializer(&m_VariableDeltaSerializer, &context);
        netSerialize(serializer);
    }
    m_VariableDeltaSerializer.EndSerialize(&context);

    return RakNet::RM3SR_BROADCAST_IDENTICALLY_FORCE_SERIALIZATION;
}

void NetworkObjectBase::Deserialize( RakNet::DeserializeParameters* deserializeParameters )
{
    RakNet::VariableDeltaSerializer::DeserializationContext context;
    RakNet::Time msgTime(deserializeParameters->timeStamp);
    float dTime = (RakNet::GetTime() - msgTime) / 1000.0f;

    m_VariableDeltaSerializer.BeginDeserialize(&context, &deserializeParameters->serializationBitstream[0]);
    {
        NetworkDeserializer serializer(&m_VariableDeltaSerializer, &context, dTime);
        netDeserialize(serializer);
    }
    m_VariableDeltaSerializer.EndDeserialize(&context);
}

void NetworkObjectBase::OnPoppedConnection( RakNet::Connection_RM3* droppedConnection )
{
    m_VariableDeltaSerializer.RemoveRemoteSystemVariableHistory(droppedConnection->GetRakNetGUID());
}

void NetworkObjectBase::OnUserReplicaPreSerializeTick( void )
{
    m_Serialize = isNetSerializeDataDirty();
    m_VariableDeltaSerializer.OnPreSerializeTick();
}


} } } //end namespace