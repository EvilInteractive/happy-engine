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
//Created: 26/01/2013

#ifndef _HE_PhysicsBinaryStream_H_
#define _HE_PhysicsBinaryStream_H_
#pragma once

#include "common/PxIO.h"

namespace he {
namespace io {
    class BinaryVisitor;
}

namespace px {

class HAPPY_ENTRY PhysicsBinaryStream : public physx::PxOutputStream, public physx::PxInputStream
{
public:
    PhysicsBinaryStream(io::BinaryVisitor* const visitor);
    virtual ~PhysicsBinaryStream();

    io::BinaryVisitor* getVisitor() const { return m_Visitor; }

    virtual physx::PxU32 read(void* dest, physx::PxU32 count);
    virtual physx::PxU32 write(const void* src, physx::PxU32 count);

private:
    io::BinaryVisitor* m_Visitor;

    //Disable default copy constructor and default assignment operator
    PhysicsBinaryStream(const PhysicsBinaryStream&);
    PhysicsBinaryStream& operator=(const PhysicsBinaryStream&);
};

} } //end namespace

#endif
