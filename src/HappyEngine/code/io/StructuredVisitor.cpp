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

bool StructuredVisitor::visit( const he::FixedString& key, Guid& value, const char* comment /*= NULL*/ )
{
    std::string guid(m_OpenType == eOpenType_Write? value.toString() : "");
    if (visit(key, guid, comment))
    {
        if (m_OpenType == eOpenType_Read)
            value = Guid(guid.c_str());
        return true;
    }
    return false;
}

bool StructuredVisitor::visit( const he::FixedString& key, vec2& value, const char* comment /*= NULL*/ )
{
    if (enterArray(key, comment))
    {
        if (enterNode(0))
        {
            visit(value.x);
            exitNode(0);
        }
        if (enterNode(1))
        {
            visit(value.y);
            exitNode(1);
        }
        exitArray(key);
        return true;
    }
    return false;
}

bool StructuredVisitor::visit( const he::FixedString& key, vec3& value, const char* comment /*= NULL*/ )
{
    if (enterArray(key, comment))
    {
        if (enterNode(0))
        {
            visit(value.x);
            exitNode(0);
        }
        if (enterNode(1))
        {
            visit(value.y);
            exitNode(1);
        }
        if (enterNode(2))
        {
            visit(value.z);
            exitNode(2);
        }
        exitArray(key);
        return true;
    }
    return false;
}

bool StructuredVisitor::visit( const he::FixedString& key, vec4& value, const char* comment /*= NULL*/ )
{
    if (enterArray(key, comment))
    {
        if (enterNode(0))
        {
            visit(value.x);
            exitNode(0);
        }
        if (enterNode(1))
        {
            visit(value.y);
            exitNode(1);
        }
        if (enterNode(2))
        {
            visit(value.z);
            exitNode(2);
        }
        if (enterNode(3))
        {
            visit(value.w);
            exitNode(3);
        }
        exitArray(key);
        return true;
    }
    return false;
}

} } //end namespace