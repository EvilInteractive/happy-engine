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
//Created: 2014/01/04
#include "HappyPCH.h"
#include "ShaderEnums.h"

namespace he {
namespace gfx {

const ShaderUniformID ShaderUniformID::Unassigned;



size_t getShaderAttributeSize( const EShaderAttributeType type, const EShaderAttributeTypeComponents components )
{
    size_t typeSize(4);
    switch (type)
    {
    case eShaderAttributeType_Float:
    case eShaderAttributeType_Int32:
    case eShaderAttributeType_UInt32: 
        typeSize = 4; break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown EShaderAttributeType when getting size! %d", type);
    }
    return typeSize * components;
}

} } //end namespace
