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

#ifndef _HE_NETWORK_MANAGER_H_
#define _HE_NETWORK_MANAGER_H_
#pragma once

#include "HeAssert.h"
#undef assert
#include "boost/asio.hpp"
#undef assert
#define assert ASSERT
#include "boost/thread.hpp"

namespace he {
namespace net {

class NetworkManager
{
public:
	NetworkManager();
    virtual ~NetworkManager();

    void start();
    void stop();

    boost::asio::io_service& getIoService();

private:

    void asioThread();

    boost::asio::io_service m_IoService;
    boost::thread m_AsioThread;

    bool m_AsioIsAlive;

    //Disable default copy constructor and default assignment operator
    NetworkManager(const NetworkManager&);
    NetworkManager& operator=(const NetworkManager&);
};

} } //end namespace

#endif
