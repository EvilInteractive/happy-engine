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
//Created: 24/08/2011

#ifndef _HE_CLIENT_H_
#define _HE_CLIENT_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert ASSERT

#include "boost/shared_ptr.hpp"
#undef assert
#include "boost/asio.hpp"
#undef assert
#define assert ASSERT
#include "HappyTypes.h"
#include "Message.h"
#include "Server.h"

namespace he {
namespace net {

class Client
{
public:
    Client();
    virtual ~Client();

    void asyncConnect(const std::string& ip, ushort port);
    void disconnect();

    typedef boost::shared_ptr<Client> pointer;

    /**
    \brief Sends a message to the server

    \param[in] msg The message to be send, buffer will be copied: no need to keep it alive

    \param[in] msgSize Size in bytes of the buffer

    @see sendUserMessage
    */
    void sendUserMessage(const void* msg, size_t msgSize);

    bool isConnected() const;
    byte getUserId() const;


protected:
    virtual void handleUserMessage(const void* msg, size_t msg_size, byte user) = 0;
    virtual void handleLoggedIn() = 0;

private:

    void asycRead();
    void sendMessage(const details::Message::pointer& msg);

    void handleConnect(const boost::system::error_code& err);
    void handleReceive(const boost::system::error_code& err, size_t byteReceived);
    void handleWrite(details::Message::pointer msg, const boost::system::error_code& err, size_t bytesWritten);

    void handleInternalMessage(void* msg, size_t msg_size, Server::Header* pHeader);

    boost::asio::ip::udp::socket* m_pUdpSocket;
    boost::asio::ip::udp::endpoint m_ServerEndpoint;
    boost::asio::ip::udp::endpoint m_LastPacketEndpoint;

    void* m_pBuffer;
    byte m_UserId;
    bool m_Connected;

    //Disable default copy constructor and default assignment operator
    Client(const Client&);
    Client& operator=(const Client&);
};

} } //end namespace

#endif
