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
//Author: Bastian Damman
//Created: 2013/06/16
#include "HappyPCH.h" 

#include "ShaderUniformFactory.h"
#include "ShaderUniform.h"
#include "ShaderEnums.h"
#include "ContentManager.h"
#include "Texture2D.h"
#include "TextureCube.h"

namespace he {
namespace gfx {

void ShaderUniformFactory::destroy(IShaderUniform* const uniform)
{
    HEDelete(uniform);
}
    
he::gfx::IShaderUniform* ShaderUniformFactory::create( const EShaderUniformType type, const he::FixedString& name, const uint32 id )
{
    IShaderUniform* uniform(nullptr);
    switch (type)
    { 
    case eShaderUniformType_Int: uniform = HENew(ShaderUniformInt)(name, id, 0); break;
    case eShaderUniformType_UInt: uniform = HENew(ShaderUniformUint)(name, id, 0); break;
    case eShaderUniformType_Float: uniform = HENew(ShaderUniformFloat)(name, id, 0.0f); break;
    case eShaderUniformType_Float2: uniform = HENew(ShaderUniformVec2)(name, id, vec2::zero); break;
    case eShaderUniformType_Float3: uniform = HENew(ShaderUniformVec3)(name, id, vec3::zero); break;
    case eShaderUniformType_Float4: uniform = HENew(ShaderUniformVec4)(name, id, vec4::zero); break;
    case eShaderUniformType_Mat44: uniform = HENew(ShaderUniformMat44)(name, id, mat44::Identity); break;
    case eShaderUniformType_Mat44Array: uniform = HENew(ShaderUniformMat44Array)(name, id); break;
    case eShaderUniformType_Texture2D: 
        {
            const Texture2D* tex(CONTENT->makeTexture2D(Color(1.0f, 0.0f, 1.0f, 1.0f)));
            uniform = HENew(ShaderUniformTexture2D)(name, id, tex);
            tex->release();
        } break;
    case eShaderUniformType_TextureCube: 
        {
            const TextureCube* tex(CONTENT->makeTextureCube(Color(1.0f, 0.0f, 1.0f, 1.0f)));
            uniform = HENew(ShaderUniformTextureCube)(name, id, tex);
            tex->release();
        } break;
    default:
        LOG(LogType_ProgrammerAssert, "Trying to create an unknown ShaderUniform type, %d", type);
        break;
    }
    return uniform;
}

} } //end namespace