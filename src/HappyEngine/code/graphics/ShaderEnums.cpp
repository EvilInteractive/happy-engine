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
    
EShaderAttributePropertyUsage bufferElementUsageToShaderAttribUsage(const BufferElement::Usage usage)
{
    EShaderAttributePropertyUsage result(eShaderAttributePropertyUsage_Invalid);
    switch (usage)
    {
        case BufferElement::Usage_Position: result = eShaderAttributePropertyUsage_Position; break;
        case BufferElement::Usage_TextureCoordinate: result = eShaderAttributePropertyUsage_TextureCoordiante; break;
        case BufferElement::Usage_Normal: result = eShaderAttributePropertyUsage_Normal; break;
        case BufferElement::Usage_Tangent: result = eShaderAttributePropertyUsage_Tangent; break;
        case BufferElement::Usage_BoneIDs: result = eShaderAttributePropertyUsage_BoneIndices; break;
        case BufferElement::Usage_BoneWeights: result = eShaderAttributePropertyUsage_BoneWeights; break;
            
        case BufferElement::Usage_Instancing:
        case BufferElement::Usage_Other:
            break;
            
        default: LOG(LogType_ProgrammerAssert, "Unknown bufferelement usage when converting to attrib usage: %d", usage); break;
    }
    return result;
}
    
} } //end namespace
