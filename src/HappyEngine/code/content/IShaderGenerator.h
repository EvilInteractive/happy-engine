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
//Created: 04/11/2012

#ifndef _HE_IShaderGenerator_H_
#define _HE_IShaderGenerator_H_
#pragma once

namespace he {
namespace ct {

enum ShaderGeneratorVariableType
{
    ShaderVariableType_Int,
    ShaderVariableType_Int2,
    ShaderVariableType_Int3,
    ShaderVariableType_Int4,
    ShaderVariableType_Uint,
    ShaderVariableType_Float,
    ShaderVariableType_Float2,
    ShaderVariableType_Float3,
    ShaderVariableType_Float4,
    ShaderVariableType_Mat44,
    ShaderVariableType_Texture2D,
    ShaderVariableType_TextureCube
};

class IShaderGeneratorVariable
{
    virtual ShaderGeneratorVariableType getType() const = 0;
    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string&) = 0;
};

class IShaderGenerator
{
public:
    virtual ~IShaderGenerator() {}

    virtual bool compile() = 0;

    virtual void addGlobalParameter(IShaderGeneratorVariable* var);
    virtual void addLocalVariable(IShaderGeneratorVariable* var);

    virtual void add(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);
    virtual void mul(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);
    virtual void mad(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);
    virtual void sub(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);
    virtual void div(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);


    virtual void sample(IShaderGeneratorVariable* out, IShaderGeneratorVariable* in1, IShaderGeneratorVariable* in2);

   
};

} } //end namespace

#endif
