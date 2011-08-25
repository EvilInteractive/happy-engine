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

#include "NetworkManager.h"
#include "HappyNew.h"

namespace happyengine {
namespace networking {

NetworkManager::NetworkManager(): m_IoService(), m_AsioIsAlive(false)
{
}


NetworkManager::~NetworkManager()
{
    if (m_AsioIsAlive)
        stop();
}

void NetworkManager::start()
{
    m_AsioIsAlive = true;
    m_AsioThread = boost::thread(boost::bind(&NetworkManager::asioThread, this));
}
void NetworkManager::asioThread()
{
    std::cout << "Asio Started\n";
    try 
    {
        m_IoService.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Unhandled exception (AsioThread): \n";
        std::cout << "  " << e.what() << "\n";
    }
    m_AsioIsAlive = false;
    std::cout << "Asio Ended\n";
}

void NetworkManager::stop()
{
    m_IoService.stop();
    m_AsioThread.join();
}

boost::asio::io_service& NetworkManager::getIoService()
{
    return m_IoService;
}

} } //end namespace