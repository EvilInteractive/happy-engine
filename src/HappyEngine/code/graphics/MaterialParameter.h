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
//Created: 2013/09/02

#ifndef _HE_MaterialParameter_H_
#define _HE_MaterialParameter_H_
#pragma once

namespace he {
namespace gfx {

class MaterialParameter
{
public:
    MaterialParameter();
    ~MaterialParameter();

    float getFloat() const;
    const vec2& getFloat2() const;
    const vec3& getFloat3() const;
    const vec4& getFloat4() const;
    int32 getInt() const;
    const FixedString& getFixedString() const;
    ObjectHandle getObjectHandle() const;

private:
    ShaderUniformID m_ID;
    enum EType
    {
        eType_Invalid,
        eType_Float,
        eType_Float2,
        eType_Float3,
        eType_Float4,
        eType_Int,
        eType_FixedString,
        eType_ObjectHandle,
    } m_Type;
    union Data
    {
        float m_Float[4];
        int32 m_Int;
        char* m_FixedString;
        uint32 m_Handle;
    } m_Data;

    HE_COMPILE_ASSERT(sizeof(uint32) == sizeof(ObjectHandle), "ObjectHandle does not have the same size as a uint32!");
    HE_COMPILE_ASSERT(sizeof(char*) == sizeof(FixedString), "FixedString does not have the same size as a char*!");

    const char* typeToString(const EType type) const;

    //Disable default copy constructor and default assignment operator
    MaterialParameter(const MaterialParameter&);
    MaterialParameter& operator=(const MaterialParameter&);
};

} } //end namespace

#endif
