//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_
#pragma once

#include "Server.h"

namespace happytest {

class MyServer : public he::net::Server
{
public:
	MyServer();
    virtual ~MyServer();

    virtual void handleClientMessage(const void* msg, he::uint msg_size, he::byte fromUser);

private:

    //Disable default copy constructor and default assignment operator
    MyServer(const MyServer&);
    MyServer& operator=(const MyServer&);
};

} //end namespace

#endif
