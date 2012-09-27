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
//Created: 25/06/2012

#ifndef _HE_INetworkSerializable_H_
#define _HE_INetworkSerializable_H_
#pragma once

namespace he {
namespace net {
class NetworkSerializer;
class NetworkDeserializer;

class INetworkSerializable
{
public:
    virtual void serializeCreate(net::NetworkStream* stream) const = 0;
    virtual bool deserializeCreate(net::NetworkStream* stream) = 0;
    virtual void serializeRemove(net::NetworkStream* stream) const = 0;
    virtual bool deserializeRemove(net::NetworkStream* stream) = 0;

    virtual bool isSerializeDataDirty() const = 0;
    virtual void serialize(const NetworkSerializer& serializer) = 0;
    virtual void deserialize(const NetworkDeserializer& serializer) = 0;
};

} } //end namespace

#endif
