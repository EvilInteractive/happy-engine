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

#ifndef _HE_NETWORK_MANAGER_H_
#define _HE_NETWORK_MANAGER_H_
#pragma once

#include "ReplicaManager3.h"
#include "NetworkConnection.h"

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

class NetworkManager : public RakNet::ReplicaManager3
{
public:
    NetworkManager();
    virtual ~NetworkManager();

    void setMaxConnections(uint8 count);

    void host(uint16 port = 30000);
    void join(const std::string& ip = "localhost", uint16 port = 30000);
    void disconnect();
    void tick(float dTime);
    bool isConnected() const;

    bool IsHost() const;
    NetworkID getNetworkId() const;

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

private:
    void clientConnected(const NetworkID& id, const std::string& adress);
    void clientDisconnected(const NetworkID& id);


    float m_Sleep, m_SleepTimout;

    NetworkObjectFactoryManager* m_NetworkObjectFactoryManager;

    ConnectionType m_ConnectionType;
    RakNet::RakPeerInterface* m_RakPeer;
    RakNet::NetworkIDManager* m_NetworkIdManager;

    uint8 m_MaxConnections;

    // Host
    std::map<NetworkID, NetworkConnection> m_Connections;

    //Disable default copy constructor and default assignment operator
    NetworkManager(const NetworkManager&);
    NetworkManager& operator=(const NetworkManager&);
};

} } //end namespace

#endif
