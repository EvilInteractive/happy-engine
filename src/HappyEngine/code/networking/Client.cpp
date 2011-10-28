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
#include "StdAfx.h" 

#include "Client.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "NetworkManager.h"

#include <iostream>

namespace he {
namespace net {

Client::Client(): m_pUdpSocket(nullptr), m_UserId(0), m_Connected(false)
{
    m_pBuffer = malloc(Server::MAX_MESSAGE_SIZE);
}


Client::~Client()
{
    if (m_pUdpSocket != nullptr)
        disconnect();
    free(m_pBuffer);
}
void Client::disconnect()
{
    //Send disconnect message
    Server::Header header;
    header.type = ServerMessage_Disconnect;
    header.user = m_UserId;

    details::Message::pointer msg(details::Message::createServerMsg(0, 0, &header, sizeof(Server::Header)));

    m_pUdpSocket->send_to(boost::asio::buffer(msg->getMsg(), msg->getSizeInBytes()),
        m_ServerEndpoint);

    //disconnect
    m_Connected = false;
    m_pUdpSocket->shutdown(boost::asio::socket_base::shutdown_both);
    m_pUdpSocket->close();
    delete m_pUdpSocket;
    m_pUdpSocket = nullptr;
}

void Client::asyncConnect(const std::string& ip, ushort port)
{
    m_Connected = true;

    using namespace boost::asio::ip;
    m_pUdpSocket = NEW udp::socket(NETWORK->getIoService());

    address_v4 address(address_v4::from_string(ip));
    m_ServerEndpoint = udp::endpoint(address, port);

    m_pUdpSocket->async_connect(m_ServerEndpoint, 
        boost::bind(&Client::handleConnect, this, boost::asio::placeholders::error));

    std::cout << "Connecting... \n";
}

void Client::handleConnect(const boost::system::error_code& err)
{
    if (!err)
    {
        std::cout << "successful connected to " << m_ServerEndpoint.address().to_string() << ":" << 
            m_ServerEndpoint.port() << "\n";
        
        asycRead();

        //Send connect request
        Server::Header header;
        header.type = ServerMessage_Connect;
        header.user = 0;
        sendMessage(details::Message::createServerMsg(0, 0, &header, sizeof(Server::Header)));
    }
    else
    {
        std::cout << "failed to connect to " << m_ServerEndpoint.address().to_string() << ":" << 
            m_ServerEndpoint.port() << "\n";
        disconnect();
    }
}
void Client::handleReceive(const boost::system::error_code& err, size_t bytesReceived)
{
    if (!err)
    {
        Server::Header* header(static_cast<Server::Header*>(m_pBuffer));
        if (header->type == ServerMessage_None)
        {
            handleUserMessage(header+1, bytesReceived - sizeof(Server::Header), header->user);
        }
        else
        {
            handleInternalMessage(header+1, bytesReceived - sizeof(Server::Header), header);
        }
        if (m_Connected)
            asycRead();
    }
    else if (err != boost::asio::error::eof)
    {
        std::cout << "Error receiving message from server (" << err << ")\n";
    }
    else
    {
        return;
    }
}
void Client::handleWrite(details::Message::pointer /*msg*/, const boost::system::error_code& error, size_t /*bytesWritten*/)
{
    if (!error)
    {
    }
    else if (error != boost::asio::error::eof)
    {
        std::cout << "Error sending message to server (" << error << ")\n";
    }
    else
    {
        return;
    }
}

void Client::asycRead()
{
    m_pUdpSocket->async_receive_from(boost::asio::buffer(m_pBuffer, Server::MAX_MESSAGE_SIZE), 
        m_LastPacketEndpoint,
        boost::bind(&Client::handleReceive, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}
void Client::sendUserMessage(const void* msg, size_t msgSize)
{
    Server::Header header;
    header.type = ServerMessage_None;
    header.user = getUserId();
    sendMessage(details::Message::createServerMsg(msg, msgSize, &header, sizeof(Server::Header)));
}
void Client::sendMessage(const details::Message::pointer& msg)
{
    m_pUdpSocket->async_send_to(boost::asio::buffer(msg->getMsg(), msg->getSizeInBytes()), m_ServerEndpoint,
        boost::bind(&Client::handleWrite, this, msg, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Client::handleInternalMessage(void* msg, size_t /*msg_size*/, Server::Header* pHeader)
{
    switch (pHeader->type)
    {
        case ServerMessage_None: ASSERT("must be handled in previous method"); break;
        case ServerMessage_Connect: 
            if ((*static_cast<byte*>(msg)) == 0)
            {
                m_UserId = pHeader->user;
                std::cout << "logged in successfull got slot: " << (int)m_UserId << "\n";
                handleLoggedIn();
            }
            else
            {
                std::cout << "faild to log in (to many users)\n";
                disconnect();
            }
            break;
        case ServerMessage_Disconnect: 
            std::cout << "server kicked you!\n";
            disconnect();
            break;
        default: ASSERT("unkown message type"); break;
    }
}
bool Client::isConnected() const
{
    return m_Connected;
}
byte Client::getUserId() const
{
    return m_UserId;
}

} } //end namespace