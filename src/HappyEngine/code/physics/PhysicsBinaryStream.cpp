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
#include "HappyPCH.h" 

#include "PhysicsBinaryStream.h"
#include "BinaryVisitor.h"

namespace he {
namespace px {

PhysicsBinaryStream::PhysicsBinaryStream(io::BinaryVisitor* const visitor)
    : m_Visitor(visitor)
{
}


PhysicsBinaryStream::~PhysicsBinaryStream()
{
}

physx::PxU32 PhysicsBinaryStream::read( void* dest, physx::PxU32 count )
{
    m_Visitor->visitBlob(dest, count);
    return count;
}

physx::PxU32 PhysicsBinaryStream::write( const void* src, physx::PxU32 count )
{
    m_Visitor->visitBlob(src, count);
    return count;
}

} } //end namespace
