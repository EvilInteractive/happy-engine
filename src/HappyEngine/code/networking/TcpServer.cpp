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
#include "HappyPCH.h" 

#include "TcpServer.h"
#include "HappyNew.h"
#include "HeAssert.h"
#include "boost/bind.hpp"
#include "HappyEngine.h"
#include "NetworkManager.h"

#include <iostream>

namespace he {
namespace net {

TcpServer::TcpServer(): m_pAcceptor(nullptr), m_IsConnectionOpen(false), m_pSocket(nullptr)
{
    m_pBuffer = malloc(MAX_BUFFER_SIZE);
}


TcpServer::~TcpServer()
{
    free(m_pBuffer);
    if (m_pAcceptor != nullptr)
    {
        delete m_pAcceptor;
    }
}

void TcpServer::start(ushort port)
{
    ASSERT(m_pAcceptor == nullptr, "m_pAcceptor is already initialized");
    m_pAcceptor = NEW boost::asio::ip::tcp::acceptor(
        NETWORK->getIoService(), 
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

    HE_INFO("TcpServer: P2P tpc accepting @" + itoa(port));
    startAccepting();

}
void TcpServer::startAccepting()
{
    ASSERT(m_pSocket == nullptr, "m_pSocket is already initialized");
    m_pSocket = NEW boost::asio::ip::tcp::socket(NETWORK->getIoService());
    m_pAcceptor->async_accept(*m_pSocket, boost::bind(&TcpServer::handleAccepted, this, boost::asio::placeholders::error));
}


void TcpServer::send( const void* data, uint sizeInBytes )
{
    m_pSocket->async_send(boost::asio::buffer(data, sizeInBytes), boost::bind(&TcpServer::handleWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TcpServer::handleAccepted( const boost::system::error_code& error )
{
    if (!error)
    {
        HE_INFO("TcpServer: connection established!");
        m_IsConnectionOpen = true;
        if (m_ConnectedCallback.empty() == false)
            m_ConnectedCallback();
        m_pSocket->async_receive(boost::asio::buffer(m_pBuffer, MAX_BUFFER_SIZE), 
            boost::bind(&TcpServer::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete m_pSocket;
        m_pSocket = nullptr;
        HE_ERROR("TcpServer: error accepting - msg: " + error.message());
        startAccepting();
    }
}

void TcpServer::handleWrite( const boost::system::error_code& error, size_t /*bytesWritten*/ )
{
    if (error)
    {
        HE_ERROR("TcpServer: error sending data - msg: " + error.message());
    }
}

void TcpServer::handleReceive( const boost::system::error_code& error, size_t byteReceived )
{
    if (!error)
    {
        if (m_ReceiveCallback.empty() == false)
            m_ReceiveCallback(byteReceived);
    }
    else
    {
        HE_ERROR("TcpServer: error receiving data - msg: " + error.message());
    }
}

bool TcpServer::isConnected() const
{
    return m_IsConnectionOpen;
}

void TcpServer::setReceiveCallback( const boost::function<void (int bytesReceived)>& callback ) 
{
    m_ReceiveCallback = callback;
}

void TcpServer::setConnectedCallback( const boost::function<void ()>& callback )
{
    m_ConnectedCallback = callback;
}




} } //end namespace