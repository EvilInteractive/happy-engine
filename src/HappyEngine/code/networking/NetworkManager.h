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

#ifndef _HE_NETWORK_MANAGER_H_
#define _HE_NETWORK_MANAGER_H_
#pragma once

#include "ReplicaManager3.h"
#include "NetworkConnection.h"
#include "NetworkPackage.h"

#include <PacketPriority.h>

namespace RakNet {
    class RakPeerInterface;
}

namespace he {
namespace net {
class INetworkObjectFactory;
class NetworkObjectFactoryManager;

enum ConnectionType 
{
    ConnectionType_Host,
    ConnectionType_Client
};

enum ENetworkPriority
{
    eNetworkPriority_Low = LOW_PRIORITY,
    eNetworkPriority_Medium = MEDIUM_PRIORITY,
    eNetworkPriority_High = HIGH_PRIORITY,
    eNetworkPriority_Immediate = IMMEDIATE_PRIORITY,
};

enum ENetworkReliability
{
    eNetworkReliability_UnReliable = UNRELIABLE,
    eNetworkReliability_UnReliableSequenced = UNRELIABLE_SEQUENCED, // discard packages if a newer has already arrived
    eNetworkReliability_Reliable = RELIABLE_ORDERED,
    eNetworkReliability_ReliableOrdered = RELIABLE_ORDERED,  // queue packages to match send order
    eNetworkReliability_ReliableSequenced = RELIABLE_SEQUENCED  // discard packages if a newer has already arrived
};

class NetworkManager : public RakNet::ReplicaManager3
{
public:
    NetworkManager();
    virtual ~NetworkManager();

    void setMaxConnections(uint8 count);

    void host(uint16 port = 30000);
    void join(const he::String& ip = "localhost", uint16 port = 30000);
    void disconnect();
    void tick(float dTime);
    bool isConnected() const;

    void send(const NetworkPackage& package, const NetworkID& to, 
        const ENetworkReliability reliability = eNetworkReliability_ReliableOrdered, const ENetworkPriority priority = eNetworkPriority_High);
    void sendToServer(const NetworkPackage& package,
        const ENetworkReliability reliability = eNetworkReliability_ReliableOrdered, const ENetworkPriority priority = eNetworkPriority_High);
    void broadcast(const NetworkPackage& package, const bool ignoreSelf = true, 
        const ENetworkReliability reliability = eNetworkReliability_ReliableOrdered, const ENetworkPriority priority = eNetworkPriority_High);

    bool IsHost() const;
    NetworkID getNetworkId() const;

    static void sdmInit();
    static void sdmDestroy();
    
    // Normal = 1 / 30s
    // Fastplay = 1 / 60s
    void setSyncTimeout(float seconds);

    NetworkObjectFactoryManager* getNetworkObjectFactoryManager() const { return m_NetworkObjectFactoryManager; }

    // Internal
    void registerFactory( INetworkObjectFactory* factory );
    virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID) const;
    virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const;

    he::event1<void, const NetworkID&> ClientConnected;
    he::event1<void, const NetworkID&> ClientDisconnected;
    he::event0<void> ConnectionSuccessful;
    he::event0<void> ConnectionFailed;
    he::event0<void> ConnectionLost;
    he::event1<bool, NetworkPackage&> PacketReceived; // return true if you handled the package

private:
    void clientConnected(const NetworkID& id, const he::String& adress);
    void clientDisconnected(const NetworkID& id);

    float m_Sleep, m_SleepTimout;

    NetworkObjectFactoryManager* m_NetworkObjectFactoryManager;

    ConnectionType m_ConnectionType;
    RakNet::RakPeerInterface* m_RakPeer;
    RakNet::NetworkIDManager* m_NetworkIdManager;

    uint8 m_MaxConnections;
    NetworkID m_ServerID;

    NetworkPackage m_NetworkPackage;

    // Host
    he::Map<NetworkID, NetworkConnection, NetworkIDHasher> m_Connections;

    //Disable default copy constructor and default assignment operator
    NetworkManager(const NetworkManager&);
    NetworkManager& operator=(const NetworkManager&);
};

} } //end namespace

#endif
