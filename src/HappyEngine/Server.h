//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 24/08/2011

#ifndef _SERVER_H_
#define _SERVER_H_
#pragma once

#include "HappyNew.h"
#include "HappyTypes.h"

#include <string>
#include <vector>
#include <queue>

#include "boost/asio.hpp"
#include "Message.h"

namespace happyengine {
namespace networking {

enum ServerMessage
{
    ServerMessage_None = 0,
    ServerMessage_Connect = 1,
    ServerMessage_Disconnect = 2
};

class Server
{
public:
	Server();
    virtual ~Server();

    void start(ushort port, byte maxConnections);
    void stop();

    static const int MAX_MESSAGE_SIZE = 1024; 

    struct Header
    {
        uint32 type;
        byte user;
    };

protected:
    virtual void handleClientMessage(const void* msg, uint msg_size, byte fromUser) = 0;
    
    void userSendMessageToAll(const void* pMsg, uint sizeInBytes, byte from);
    void userSendMessageToAllBut(const void* pMsg, uint sizeInBytes, byte from, byte userId);
    void userSendMessageToUser(const void* pMsg, uint sizeInBytes, byte from, byte userId);

    const std::vector<byte>& getConnectedUsers() const;

private:
    struct User
    {
    public:
        uint id;
        boost::asio::ip::udp::endpoint endpoint;
        bool connected;

        User(byte id_, const boost::asio::ip::udp::endpoint& endpoint_):
            id(id_), endpoint(endpoint_), connected(true)
        {
        }
        User(): id(0), endpoint(), connected(false)
        {
        }
    };

    //------Private Methods------------------------------------------
    void handleReceive(const boost::system::error_code& error, size_t byteReceived);
    void handleServerMessage(void* msg, size_t msg_size, Header* pHeader);
    void asycRead();

    void userConnecting();
    void userDisconnecting(byte userId);

    void sendMessageToAll(const details::Message::pointer& msg);
    void sendMessageToAllBut(const details::Message::pointer& msg, byte userId);
    void sendMessageToUser(const details::Message::pointer& msg, byte userId);
    void sendMessageToCurrent(const details::Message::pointer& msg);

    void handleWrite(
        details::Message::pointer msg, byte userId,
        const boost::system::error_code& error, size_t bytesWritten);

    //------Private Members-------------------------------------------
    std::vector<User> m_Users;
    std::vector<byte> m_ConnectedUsers;
    std::queue<byte> m_FreeSlots;

    boost::asio::ip::udp::socket* m_pUdpSocket;
    boost::asio::ip::udp::endpoint m_LastPacketSender;
    void* m_pBuffer;

    //----------------------------------------------------------------
    //Disable default copy constructor and default assignment operator
    Server(const Server&);
    Server& operator=(const Server&);
};

} } //end namespace

#endif
