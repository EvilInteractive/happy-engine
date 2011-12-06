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
//Created: 25/08/2011
#include "HappyPCH.h" 

#include "Message.h"
#include "HappyNew.h"
#include "HeAssert.h"

namespace he {
namespace net {
namespace details {

Message::Message(): m_pMsg(nullptr), m_SizeInBytes(0)
{
}

Message::pointer Message::createServerMsg(const void* msg, uint sizeInBytes, const void* header, uint headerSize)
{
    ASSERT(header != nullptr, "header == nullptr");
    ASSERT(sizeInBytes + headerSize != 0, "msg + header size == 0");
    ASSERT(!(sizeInBytes > 0 && msg == nullptr), "msg == nullptr");

    Message::pointer pMsg(NEW Message());
    pMsg->m_pMsg = malloc(sizeInBytes + headerSize);
    memcpy(pMsg->m_pMsg, header, headerSize);
    if (sizeInBytes > 0)
        memcpy(&static_cast<byte*>(pMsg->m_pMsg)[headerSize], msg, sizeInBytes);

    pMsg->m_SizeInBytes = sizeInBytes + headerSize;

    return pMsg;
}
Message::pointer Message::createServerMsg(const void* msg, uint sizeInBytes)
{
    ASSERT(msg != nullptr, "msg == nullptr");
    ASSERT(sizeInBytes != 0, "msg size == 0");

    Message::pointer pMsg(NEW Message());
    pMsg->m_pMsg = malloc(sizeInBytes);
    memcpy(pMsg->m_pMsg, msg, sizeInBytes);

    pMsg->m_SizeInBytes = sizeInBytes;

    return pMsg;
}

Message::~Message()
{
    free(m_pMsg);
}

void* Message::getMsg() const
{
    return m_pMsg;
}
uint Message::getSizeInBytes() const
{
    return m_SizeInBytes;
}

} } } //end namespace