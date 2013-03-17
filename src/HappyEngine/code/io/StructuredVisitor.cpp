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
//Created: 26/01/2013
#include "HappyPCH.h"

#include "StructuredVisitor.h"

namespace he {
namespace io {

template<>
void StructuredVisitor::visit(he::String& value)
{
    uint32 length(checked_numcast<uint32>(value.size()));
    visit(length);
    if (m_OpenType == eOpenType_Read)
    {
        value.resize(length);
        readBuffer(&value[0], sizeof(char) * length);
    }
    else
    {
        writeBuffer(&value[0], sizeof(char) * length);
    }
}

} } //end namespace
