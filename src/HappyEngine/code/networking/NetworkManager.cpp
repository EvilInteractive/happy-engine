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

#define RAK_SLEEP 1 / 30.0f
#define SERVER_PORT 30000
#define SERVER_IP "localhost"
#define MAX_CONNECTIONS 16

namespace he {
namespace net {

NetworkManager::NetworkManager(): m_RakPeer(nullptr), m_Sleep(0.0f), m_ConnectionType(ConnectionType_Client)
{
}


NetworkManager::~NetworkManager()
{
    bool isConnected(isConnected());
    HE_ASSERT(isConnected == false, "Net: Disconnect before quiting!");
    if (isConnected == true)
        disconnect();
}

void NetworkManager::host()
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = SERVER_PORT;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(32);
        HE_INFO("Net: Starting server @ %s:%d...", SERVER_IP, SERVER_PORT);

        m_ConnectionType = ConnectionType_Host;
    }
}

void NetworkManager::join()
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = 0;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(32);
        m_RakPeer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
        HE_INFO("Net: Connecting to %s:%d...", SERVER_IP, SERVER_PORT);

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

void NetworkManager::tick( float dTime )
{
    if (m_RakPeer == nullptr)
        return;
    m_Sleep += dTime;
    if (m_Sleep > RAK_SLEEP)
    {
        m_Sleep = 0.0f; // don't care if m_Sleep is x * RAK_SLEEP

        for (RakNet::Packet* packet(m_RakPeer->Receive());   packet != nullptr; 
                m_RakPeer->DeallocatePacket(packet), packet = m_RakPeer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_CONNECTION_ATTEMPT_FAILED:
                HE_WARNING("Net: Connection attempt failed!");
                disconnect();
                return;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                HE_WARNING("Net: Connection failed, no free incoming connections!");
                disconnect();
                return;
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                HE_INFO("Net: Connection accepted!");
                break;
            case ID_NEW_INCOMING_CONNECTION:
                HE_INFO("Net: new connection from %s", packet->systemAddress.ToString());
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                if (m_ConnectionType == ConnectionType_Host)
                {
                    HE_INFO("Net: client disconnected: %s", packet->systemAddress.ToString());
                }
                else
                {
                    HE_INFO("Net: Server has been shutdown");
                    disconnect();
                    return;
                }
                break;
            case ID_CONNECTION_LOST:
                HE_INFO("Net: client connection lost: %s", packet->systemAddress.ToString());
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


} } //end namespace