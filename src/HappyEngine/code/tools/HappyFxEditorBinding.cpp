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
//Created: 23/11/2011
#include "StdAfx.h" 

#include "HappyFxEditorBinding.h"
#include "HappyNew.h"

#include "boost/bind.hpp"

#include "HappyEngine.h"
#include "NetworkManager.h"

namespace he {
namespace tools {

HappyFxEditorBinding::HappyFxEditorBinding(): m_pServer(NEW net::TcpServer())
{
}



HappyFxEditorBinding::~HappyFxEditorBinding()
{
    delete m_pServer;
}

void HappyFxEditorBinding::init()
{
    m_pServer->setReceiveCallback(boost::bind(&HappyFxEditorBinding::receiveData, this, _1));
    m_pServer->setConnectedCallback(boost::bind(&HappyFxEditorBinding::connected, this));
    start();
}

void HappyFxEditorBinding::receiveData( int bytesReceived )
{

}

void HappyFxEditorBinding::start()
{
    m_pServer->start(3250);
    NETWORK->start();
}

void HappyFxEditorBinding::connected()
{
    std::string str("Hallo from C++!");
    m_pServer->send(str.c_str(), str.size()*sizeof(char));
    std::string str2("Hallo from C++ v2!");
    m_pServer->send(str2.c_str(), str2.size()*sizeof(char));
}

} } //end namespace