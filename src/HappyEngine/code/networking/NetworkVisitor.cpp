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
//Created: 23/02/2013
#include "HappyPCH.h"

#include "NetworkVisitor.h"

namespace he {
namespace net {

template<>
bool NetworkVisitor::visit(he::String& value)
{
    bool result(true);
    uint16 length(checked_numcast<uint16>(value.size()));
    result &= visit(length);
    if (m_OpenType == eOpenType_Read)
    {
        value.resize(length);
        result &= m_Stream->Read(&value[0], sizeof(char) * length);
    }
    else
    {
        m_Stream->Write(&value[0], sizeof(char) * length);
    }
    return result;
}


} } //end namespace
