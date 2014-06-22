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
//Created: 13/03/2013
#include "HappyPCH.h" 

#include "FixedString.h"
#include "GlobalStringTable.h"

namespace he {

FixedString::FixedString( const FixedString& other )
    : m_Handle(other.m_Handle)
{

}

FixedString::FixedString( const char* const str )
    : m_Handle(str)
{

}

FixedString& FixedString::operator=( const FixedString& other )
{
    m_Handle = other.m_Handle;
    return *this;
}

FixedString::~FixedString()
{
}

he::FixedString FixedString::fromString( const char* const str, const int len /*= -1*/ )
{
    return GlobalStringTable::getInstance()->add(str, len);
}

} //end namespace
