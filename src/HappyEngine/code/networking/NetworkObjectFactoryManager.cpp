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
//Created: 23/06/2012
#include "HappyPCH.h" 

#include "NetworkObjectFactoryManager.h"
#include "NetworkObjectBase.h"
#include "NetworkObjectFactory.h"

namespace he {
namespace net {

NetworkObjectFactoryManager::NetworkObjectFactoryManager()
{

}


NetworkObjectFactoryManager::~NetworkObjectFactoryManager()
{
}

void NetworkObjectFactoryManager::registerFactory( INetworkObjectFactory* factory )
{
    HE_IF_ASSERT(factory->getId() == NetworkObjectTypeID::unassigned, "factory already registered!")
    {
        factory->setId(static_cast<uint16>(m_Factories.size()));
        m_Factories.add(factory);
    }
}

details::NetworkObjectBase* NetworkObjectFactoryManager::createObject( const NetworkObjectTypeID& id )
{
    details::NetworkObjectBase* result(nullptr);
    HE_IF_ASSERT(id.id < m_Factories.size(), "Invalid network ID!")
    {
        result = m_Factories[id.id]->createReplica();
    }
    return result;
}


} } //end namespace