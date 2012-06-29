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

#ifndef _HE_NetworkSerializer_H_
#define _HE_NetworkSerializer_H_
#pragma once

#include "VariableDeltaSerializer.h"

namespace he {
namespace net {

class NetworkSerializer
{
public:
    NetworkSerializer(RakNet::VariableDeltaSerializer* internalSerializer, 
        RakNet::VariableDeltaSerializer::SerializationContext* context);
    ~NetworkSerializer();

    template <class T>
    void serializeVariable(const T& variable)
    {
        m_InternalSerializer->SerializeVariable(m_Context, variable);
    }

private:
    RakNet::VariableDeltaSerializer* m_InternalSerializer;
    RakNet::VariableDeltaSerializer::SerializationContext* m_Context;
};

class NetworkDeserializer
{
public:
    NetworkDeserializer(RakNet::VariableDeltaSerializer* internalSerializer, 
        RakNet::VariableDeltaSerializer::DeserializationContext* context,
        float dTime);
    ~NetworkDeserializer();
    
    // Returns true is variable is changed
    template <class T>
    bool deserializeVariable(T& variable)
    {
        return m_InternalSerializer->DeserializeVariable(m_Context, variable);
    }

    float getDTime() const { return m_DTime; }

private:
    float m_DTime;
    RakNet::VariableDeltaSerializer* m_InternalSerializer;
    RakNet::VariableDeltaSerializer::DeserializationContext* m_Context;
};

} } //end namespace

#endif
