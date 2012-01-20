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

#ifndef _HE_TcpServer_H_
#define _HE_TcpServer_H_
#pragma once

#include "HappyNew.h"
#include "HappyTypes.h"

#include <string>
#include <vector>
#include <queue>

#undef assert
#include "boost/asio.hpp"
#undef assert
#define assert ASSERT
#include "boost/function.hpp"
#include "Message.h"

namespace he {
namespace net {

class TcpServer
{
public:
    TcpServer();
    virtual ~TcpServer();

    void start(ushort port);

    static const int MAX_BUFFER_SIZE = 1024;

    bool isConnected() const;

    void send(const void* data, uint sizeInBytes);
    void setReceiveCallback(const boost::function<void (const void* buffer, int bytesReceived)>& callback);
    void setConnectedCallback(const boost::function<void ()>& callback);

private:
    //------Private Methods------------------------------------------
    void startAccepting();
    void handleAccepted(const boost::system::error_code& err);
    void handleReceive(const boost::system::error_code& err, size_t byteReceived);

    void handleWrite(
        const boost::system::error_code& err, size_t bytesWritten);

    //------Private Members-------------------------------------------
    boost::asio::ip::tcp::acceptor* m_pAcceptor;
    boost::asio::ip::tcp::socket* m_pSocket;

    bool m_IsConnectionOpen;
    bool m_IsShuttingDown;

    void* m_pBuffer;

    boost::function<void (const void*, int)> m_ReceiveCallback;
    boost::function<void ()> m_ConnectedCallback;

    //----------------------------------------------------------------
    //Disable default copy constructor and default assignment operator
    TcpServer(const TcpServer&);
    TcpServer& operator=(const TcpServer&);
};

} } //end namespace

#endif
