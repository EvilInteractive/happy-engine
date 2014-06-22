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
//Created: 26/01/2013
#include "HappyPCH.h" 

#include "NetworkPackage.h"

#include <MessageIdentifiers.h>

namespace he {
namespace net {

NetworkPackage::NetworkPackage() 
    : m_MessageID(0), m_Sender(UNASSIGNED_NETWORKID)
{
}

void NetworkPackage::init( const uint8 id )
{
    m_MessageID = id;
    m_Visitor.openWrite();
    uint8 rakID(checked_numcast<uint8>(m_MessageID + ID_USER_PACKET_ENUM));
    m_Visitor.visit(rakID);
}

void NetworkPackage::init( const uint8* data, const size_t byteCount, const NetworkID from )
{
    m_Sender = from;
    m_Visitor.openWrite();
    m_Visitor.visitBlob(data, 1);
    const uint8* realData(data + 1);
    m_Visitor.visitBlob(realData, byteCount);
    m_Visitor.close();
    m_Visitor.openRead();
    m_Visitor.visit(m_MessageID);
    m_MessageID = checked_numcast<uint8>(m_MessageID - ID_USER_PACKET_ENUM);
}

NetworkPackage::~NetworkPackage()
{
    if (m_Visitor.isOpen())
    {
        m_Visitor.close();
    }
}

void NetworkPackage::finalise()
{
    m_Visitor.close();
}


} } //end namespace
