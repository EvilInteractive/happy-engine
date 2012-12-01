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
//Created: 14/11/2012
#include "HappyPCH.h" 

#include "MaterialGeneratorNodeTypes.h"

namespace he {
namespace tools {
    
const char* materialGeneratorVariableTypeToString( const MaterialGeneratorVariableType type)
{
    const char* result("Unknown Type");
    switch (type)
    {
    case MaterialGeneratorVariableType_Float:
        {
            result = "Float";
        } break;
    case MaterialGeneratorVariableType_Float2:
        {
            result = "Float2";
        } break;
    case MaterialGeneratorVariableType_Float3:
        {
            result = "Float3";
        } break;
    case MaterialGeneratorVariableType_Float4:
        {
            result = "Float4";
        } break;
    case MaterialGeneratorVariableType_Texture2D:
        {
            result = "Texture2D";
        } break;
    case MaterialGeneratorVariableType_TextureCube:
        {
            result = "TextureCube";
        } break;
    default:
        LOG(LogType_ProgrammerAssert, "Unknown MaterialGeneratorNodeType: %d", type);
        break;
    }
    return result;
}

} } //end namespace
