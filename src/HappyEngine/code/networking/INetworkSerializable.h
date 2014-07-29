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
//Created: 25/06/2012

#ifndef _HE_INetworkSerializable_H_
#define _HE_INetworkSerializable_H_
#pragma once

namespace he {
namespace net {
class NetworkSerializer;
class NetworkDeserializer;
class NetworkVisitor;

class INetworkSerializable
{
public:
    virtual void netVisitCreate(net::NetworkVisitor& stream) = 0;
    virtual void netVisitRemove(net::NetworkVisitor& stream) = 0;

    virtual bool isNetSerializeDataDirty() const = 0;
    virtual void netSerialize(const NetworkSerializer& serializer) = 0;
    virtual void netDeserialize(const NetworkDeserializer& serializer) = 0;
};

} } //end namespace

#endif
