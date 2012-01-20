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
//Created: 25/08/2011

#ifndef _HE_MESSAGE_H_
#define _HE_MESSAGE_H_
#pragma once

#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"

namespace he {
namespace net {
namespace details {

/*
 * Message will take a copy of the msg you provide and keeps it alive as needed
 * Use Message::create to initialize a Message
 *
 */
class Message
{
public:
    virtual ~Message();

    typedef boost::shared_ptr<Message> pointer;
    /*
     * Internal use only!
     */
    static Message::pointer createServerMsg(const void* msg, uint sizeInBytes, const void* header, uint headerSize);
    static Message::pointer createServerMsg(const void* msg, uint sizeInBytes);

    void* getMsg() const;
    uint getSizeInBytes() const;

private:
    Message();

    void* m_pMsg;
    uint m_SizeInBytes;

    //Disable default copy constructor and default assignment operator
    Message(const Message&);
    Message& operator=(const Message&);
};

} } } //end namespace

#endif
