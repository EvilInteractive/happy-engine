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

#include "NetworkManager.h"

#include "RakPeerInterface.h"
#include "RakPeer.h"
#include "MessageIdentifiers.h"
#include "NetworkObjectFactoryManager.h"
#include "NetworkReplicaConnection.h"
#include "NetworkIdManager.h"

namespace he {
namespace net {

NetworkManager::NetworkManager()
    : m_RakPeer(nullptr)
    , m_Sleep(0.0f)
    , m_ConnectionType(ConnectionType_Client)
    , m_NetworkObjectFactoryManager(NEW NetworkObjectFactoryManager())
    , m_MaxConnections(8)
    , m_NetworkIdManager(NEW RakNet::NetworkIDManager())
    , m_SleepTimout(1 / 30.0f)
{
    SetNetworkIDManager(m_NetworkIdManager);
}


NetworkManager::~NetworkManager()
{
    delete m_NetworkObjectFactoryManager;
    delete m_NetworkIdManager;
    bool isConnected(isConnected());
    if (isConnected == true)
        disconnect();
}

void NetworkManager::host(ushort port)
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = port;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(m_MaxConnections, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(m_MaxConnections);
        m_RakPeer->AttachPlugin(this);
        HE_INFO("Net: Starting server @ %s:%d...", "localhost", port);

        m_ConnectionType = ConnectionType_Host;
    }
}

void NetworkManager::join(const std::string& ip, ushort port)
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = 0;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(m_MaxConnections, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(m_MaxConnections);
        m_RakPeer->Connect(ip.c_str(), port, 0, 0);
        m_RakPeer->AttachPlugin(this);
        HE_INFO("Net: Connecting to %s:%d...", ip.c_str(), port);

        m_ConnectionType = ConnectionType_Client;
    }
}

void NetworkManager::disconnect()
{
    HE_IF_ASSERT(isConnected(), "Already disconnected or never had a connect attempt!")
    {
        m_RakPeer->Shutdown(100, 0);
        RakNet::RakPeerInterface::DestroyInstance(m_RakPeer);
        m_RakPeer = nullptr;
        HE_INFO("Net: disconnected");
    }
}
void NetworkManager::clientDisconnected( const NetworkID& id )
{
    m_Connections.erase(id);
    ClientDisconnected(id);
}
void NetworkManager::clientConnected( const NetworkID& id, const std::string& adress )
{
    NetworkConnection connection;
    connection.m_NetworkId = id;
    connection.m_SystemAdress = adress;
    m_Connections[id] = connection;
    ClientConnected(id);
}

void NetworkManager::tick( float dTime )
{
    if (m_RakPeer == nullptr)
        return;
    m_Sleep += dTime;
    if (m_Sleep > m_SleepTimout)
    {
        m_Sleep = 0.0f; // don't care if m_Sleep is x * m_SleepTimout

        for (RakNet::Packet* packet(m_RakPeer->Receive());   packet != nullptr; 
                m_RakPeer->DeallocatePacket(packet), packet = m_RakPeer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_CONNECTION_ATTEMPT_FAILED: // client
                HE_WARNING("Net: Connection attempt failed!");
                disconnect();
                ConnectionFailed();
                return;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS: // client
                HE_WARNING("Net: Connection failed, no free incoming connections!");
                disconnect();
                ConnectionFailed();
                return;
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED: // client
                HE_INFO("Net: Connection accepted!");
                ConnectionSuccessful();
                break;
            case ID_NEW_INCOMING_CONNECTION: // server
                HE_INFO("Net: new connection from %s", packet->systemAddress.ToString());
                clientConnected(packet->guid, packet->systemAddress.ToString());
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                if (m_ConnectionType == ConnectionType_Host) // server
                {
                    HE_INFO("Net: client disconnected: %s", packet->systemAddress.ToString());
                    clientDisconnected(packet->guid);
                }
                else // client
                {
                    HE_INFO("Net: Server has been shutdown");
                    disconnect();
                    ConnectionLost();
                    return;
                }
                break;
            case ID_CONNECTION_LOST: // server
                HE_INFO("Net: client connection lost: %s", packet->systemAddress.ToString());
                clientDisconnected(packet->guid);
                break;
            case ID_ADVERTISE_SYSTEM:
                if (m_RakPeer->GetSystemAddressFromGuid(packet->guid) == RakNet::UNASSIGNED_SYSTEM_ADDRESS && // Check if not already connected
                    m_RakPeer->GetMyGUID() != packet->guid) // Check if not connecting to ourself
                {
                    HE_INFO("Net: Connecting to %s\n", packet->systemAddress.ToString(true));
                    m_RakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
                }
                break;
            case ID_SND_RECEIPT_LOSS:
            case ID_SND_RECEIPT_ACKED:
                {
                    /*uint32_t msgNumber;
                    memcpy(&msgNumber, packet->data+1, 4);

                    DataStructures::List<Replica3*> replicaListOut;
                    replicaManager.GetReplicasCreatedByMe(replicaListOut);
                    unsigned int idx;
                    for (idx=0; idx < replicaListOut.Size(); idx++)
                    {
                        ((SampleReplica*)replicaListOut[idx])->NotifyReplicaOfMessageDeliveryStatus(packet->guid,msgNumber, packet->data[0]==ID_SND_RECEIPT_ACKED);
                    }*/
                }
                break;
            }
        }
    }
}

bool NetworkManager::isConnected() const
{
    return m_RakPeer != nullptr;
}

RakNet::Connection_RM3* NetworkManager::AllocConnection( const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID ) const
{
    return NEW NetworkReplicaConnection(systemAddress, rakNetGUID);
}

void NetworkManager::DeallocConnection( RakNet::Connection_RM3 *connection ) const
{
    delete connection;
}

bool NetworkManager::IsHost() const
{
    return m_ConnectionType == ConnectionType_Host;
}

NetworkID NetworkManager::getNetworkId() const
{
    if (m_RakPeer != nullptr)
        return m_RakPeer->GetMyGUID();
    else
        return UNASSIGNED_NETWORKID;
}

void NetworkManager::setMaxConnections( uint8 count )
{
    m_MaxConnections = count;
}

void NetworkManager::registerFactory( INetworkObjectFactory* factory )
{
    m_NetworkObjectFactoryManager->registerFactory(factory);
}

void NetworkManager::setSyncTimeout( float seconds )
{
    m_SleepTimout = seconds;
    SetAutoSerializeInterval(static_cast<RakNet::Time>(seconds * 1000 - 1));
}


} } //end namespace