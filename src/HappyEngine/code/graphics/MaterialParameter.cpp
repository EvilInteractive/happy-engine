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
//Author:  
//Created: //
#include "HappyPCH.h" 

#include "MaterialParameter.h"

namespace he {
namespace gfx {

MaterialParameter::MaterialParameter()
    : m_Type(eType_Invalid)
{
    he_memset(&m_Data, 0, sizeof(Data));
}


MaterialParameter::~MaterialParameter()
{
}

float MaterialParameter::getFloat() const
{
    HE_ASSERT(m_Type == eType_Float, "Trying to get a Float from a material parameter while it is actually a %s", typeToString(m_Type));
    return m_Data.m_Float[0];
}

const vec2& MaterialParameter::getFloat2() const
{
    HE_ASSERT(m_Type == eType_Float2, "Trying to get a Float2 from a material parameter while it is actually a %s", typeToString(m_Type));
    return *reinterpret_cast<const vec2*>(m_Data.m_Float); // Sorry we are missing unrestricted union feature of c++11
}

const vec3& MaterialParameter::getFloat3() const
{
    HE_ASSERT(m_Type == eType_Float3, "Trying to get a Float3 from a material parameter while it is actually a %s", typeToString(m_Type));
    return *reinterpret_cast<const vec3*>(m_Data.m_Float); // Sorry we are missing unrestricted union feature of c++11
}

const vec4& MaterialParameter::getFloat4() const
{
    HE_ASSERT(m_Type == eType_Float4, "Trying to get a Float4 from a material parameter while it is actually a %s", typeToString(m_Type));
    return *reinterpret_cast<const vec4*>(m_Data.m_Float); // Sorry we are missing unrestricted union feature of c++11
}

he::int32 MaterialParameter::getInt() const
{
    HE_ASSERT(m_Type == eType_Int, "Trying to get a Int from a material parameter while it is actually a %s", typeToString(m_Type));
    return m_Data.m_Int;
}

const FixedString& MaterialParameter::getFixedString() const
{
    HE_ASSERT(m_Type == eType_FixedString, "Trying to get a FixedString from a material parameter while it is actually a %s", typeToString(m_Type));
    return *reinterpret_cast<const FixedString*>(&m_Data.m_FixedString); // Sorry we are missing unrestricted union feature of c++11
}

he::ObjectHandle MaterialParameter::getObjectHandle() const
{
    HE_ASSERT(m_Type == eType_ObjectHandle, "Trying to get a ObjectHandle from a material parameter while it is actually a %s", typeToString(m_Type));
    return *reinterpret_cast<const ObjectHandle*>(m_Data.m_Handle); // Sorry we are missing unrestricted union feature of c++11
}

const char* MaterialParameter::typeToString( const EType type ) const
{
    const char* result("Unknown type");
    switch (type)
    {
    case eType_Invalid: result = "Invalid"; break;
    case eType_Float: result = "Float"; break;
    case eType_Float2: result = "Float2"; break;
    case eType_Float3: result = "Float3"; break;
    case eType_Float4: result = "Float4"; break;
    case eType_Int: result = "Int"; break;
    case eType_FixedString: result = "FixedString"; break;
    case eType_ObjectHandle: result = "ObjectHandle"; break;
    default: LOG(LogType_ProgrammerAssert, "Unknown MaterialParameter::EType %d when converting to string!", type);
    }
    return result;
}

} } //end namespace
