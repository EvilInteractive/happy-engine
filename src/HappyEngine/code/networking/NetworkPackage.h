//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 26/01/2013

#ifndef _HE_NetworkPackage_H_
#define _HE_NetworkPackage_H_
#pragma once

#include "BinaryStreamVisitor.h"

namespace he {
namespace net {

enum NetworkPackageID
{
    NetworkPackageID_Unknown = 0,
    NetworkPackageID_Reserved = 10, // engine reserved

    NetworkPackageID_User // start ID's from here
};

class NetworkPackage
{
public:
    NetworkPackage(const uint8* data, const size_t byteCount, const NetworkID from); // Read
    explicit NetworkPackage(const uint8 id); // Write
    ~NetworkPackage();

    uint8 getID() const { return m_MessageID; }

    const void* getData() const { return m_Visitor.getData(); }
    size_t getByteCount() const { return m_Visitor.getByteCount(); }

    NetworkID getSender() const { return m_Sender; }

    io::BinaryStreamVisitor* getVisitor() { return &m_Visitor; }
    
    void finalise();

private:
    io::BinaryStreamVisitor m_Visitor;
    uint8 m_MessageID;
    NetworkID m_Sender;
};

} } //end namespace

#endif
