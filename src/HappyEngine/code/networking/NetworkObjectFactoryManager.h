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
//Created: 23/06/2012

#ifndef _HE_NETWORK_OBJECT_FACTORY_MANAGER_H_
#define _HE_NETWORK_OBJECT_FACTORY_MANAGER_H_
#pragma once

namespace he {
namespace net {
namespace details {
class NetworkObjectBase;
}
class INetworkObjectFactory;

class NetworkObjectFactoryManager
{
public:
    NetworkObjectFactoryManager();
    virtual ~NetworkObjectFactoryManager();

    void registerFactory(INetworkObjectFactory* factory);
    details::NetworkObjectBase* createObject(const NetworkObjectTypeID& id);

private:
    he::PrimitiveList<INetworkObjectFactory*> m_Factories;

    //Disable default copy constructor and default assignment operator
    NetworkObjectFactoryManager(const NetworkObjectFactoryManager&);
    NetworkObjectFactoryManager& operator=(const NetworkObjectFactoryManager&);
};

} } //end namespace

#endif
