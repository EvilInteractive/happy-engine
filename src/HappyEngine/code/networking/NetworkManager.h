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

namespace RakNet {
    class RakPeerInterface;
}

namespace he {
namespace net {
class NetworkObjectFactory;
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

    void host();
    void join();
    void disconnect();
    void tick(float dTime);
    bool isConnected() const;

    NetworkObjectFactoryManager* getNetworkObjectFactoryManager() const;

    // Internal
    void registerFactory( NetworkObjectFactory* factory );
    virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID) const;
    virtual void DeallocConnection(RakNet::Connection_RM3 *connection) const;

private:
    float m_Sleep;

    ConnectionType m_ConnectionType;
    RakNet::RakPeerInterface* m_RakPeer;

    //Disable default copy constructor and default assignment operator
    NetworkManager(const NetworkManager&);
    NetworkManager& operator=(const NetworkManager&);
};

} } //end namespace

#endif
