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
//Created: 2013/09/02
#include "HappyPCH.h" 

#include "MaterialParameter.h"
#include "Texture2D.h"
#include "TextureCube.h"

namespace he {
namespace gfx {

MaterialParameter::MaterialParameter()
    : m_Type(eType_Invalid)
{
    he_memset(&m_Data, 0, sizeof(Data));
}
     
MaterialParameter& MaterialParameter::operator=(const MaterialParameter& other)
{
    m_ID = other.m_ID;
    m_Type = other.m_Type;
    he_memcpy(&m_Data, &other.m_Data, sizeof(Data));
    instantiate();
    return *this;
}

MaterialParameter::~MaterialParameter()
{
    release();
}
    
void MaterialParameter::init(const ShaderUniformID id, const EType type)
{
    HE_ASSERT(m_Type == eType_Invalid, "MaterialParam is already initialized!");
    HE_ASSERT(type != eType_Invalid, "Initializing materialparam with ivalid type!");
    m_ID = id;
    m_Type = type;
}
    
void MaterialParameter::instantiate()
{
    switch (m_Type)
    {
        case eType_Texture2D:
        {
            const ObjectHandle oldHandle(m_Data.m_Handle);
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureFactory::getInstance()->instantiate(oldHandle);
            }
        } break;
        case eType_TextureCube:
        {
            const ObjectHandle oldHandle(m_Data.m_Handle);
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureCubeFactory::getInstance()->instantiate(oldHandle);
            }
        } break;
        default: break;
    }
}
    
void MaterialParameter::release()
{
    switch (m_Type)
    {
        case eType_Texture2D:
        {
            const ObjectHandle oldHandle(m_Data.m_Handle);
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureFactory::getInstance()->release(oldHandle);
            }
        } break;
        case eType_TextureCube:
        {
            const ObjectHandle oldHandle(m_Data.m_Handle);
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureCubeFactory::getInstance()->release(oldHandle);
            }
        } break;
        default: break;
    }
}
    
void MaterialParameter::setFloat(const float data)
{
    HE_IF_ASSERT(m_Type == eType_Float || m_Type == eType_Invalid, "Trying to set a Float to a material parameter but its type is %s", typeToString(m_Type))
    {
        m_Data.m_Float[0] = data;
    }
}

void MaterialParameter::setFloat2(const vec2& data)
{
    HE_IF_ASSERT(m_Type == eType_Float2 || m_Type == eType_Invalid, "Trying to set a Float2 to a material parameter but its type is %s", typeToString(m_Type))
    {
        he_memcpy(m_Data.m_Float, &data, sizeof(float) * 2);
    }
}
    
void MaterialParameter::setFloat3(const vec3& data)
{
    HE_IF_ASSERT(m_Type == eType_Float3 || m_Type == eType_Invalid, "Trying to set a Float3 to a material parameter but its type is %s", typeToString(m_Type))
    {
        he_memcpy(m_Data.m_Float, &data, sizeof(float) * 3);
    }
}
    
void MaterialParameter::setFloat4(const vec4& data)
{
    HE_IF_ASSERT(m_Type == eType_Float4 || m_Type == eType_Invalid, "Trying to set a Float4 to a material parameter but its type is %s", typeToString(m_Type))
    {
        he_memcpy(m_Data.m_Float, &data, sizeof(float) * 4);
    }
}
    
void MaterialParameter::setInt32(const int32 data)
{
    HE_IF_ASSERT(m_Type == eType_Int || m_Type == eType_Invalid, "Trying to set an Int32 to a material parameter but its type is %s", typeToString(m_Type))
    {
        m_Data.m_Int = data;
    }
}
    
void MaterialParameter::setTexture2D(const Texture2D* const data)
{
    HE_IF_ASSERT(m_Type == eType_Texture2D || m_Type == eType_Invalid, "Trying to set a Texture2D to a material parameter but its type is %s", typeToString(m_Type))
    {
        const ObjectHandle oldHandle(m_Data.m_Handle);
        if (oldHandle != data->getHandle())
        {
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureFactory::getInstance()->release(oldHandle);
            }
            if (data != nullptr)
            {
                data->instantiate();
                m_Data.m_Handle = data->getHandle().m_Handle;
            }
            else
            {
                m_Data.m_Handle = ObjectHandle::unassigned.m_Handle;
            }
        }
    }
}
    
void MaterialParameter::setTextureCube(const TextureCube* const data)
{
    HE_IF_ASSERT(m_Type == eType_TextureCube || m_Type == eType_Invalid, "Trying to set a eType_TextureCube to a material parameter but its type is %s", typeToString(m_Type))
    {
        const ObjectHandle oldHandle(m_Data.m_Handle);
        if (oldHandle != data->getHandle())
        {
            if (oldHandle != ObjectHandle::unassigned)
            {
                TextureCubeFactory::getInstance()->release(oldHandle);
            }
            if (data != nullptr)
            {
                data->instantiate();
                m_Data.m_Handle = data->getHandle().m_Handle;
            }
            else
            {
                m_Data.m_Handle = ObjectHandle::unassigned.m_Handle;
            }
        }
    }
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

he::ObjectHandle MaterialParameter::getTexture2D() const
{
    HE_ASSERT(m_Type == eType_Texture2D, "Trying to get a Texture2D from a material parameter while it is actually a %s", typeToString(m_Type));
    return ObjectHandle(m_Data.m_Handle);
}
    
he::ObjectHandle MaterialParameter::getTextureCube() const
{
    HE_ASSERT(m_Type == eType_TextureCube, "Trying to get a TextureCube from a material parameter while it is actually a %s", typeToString(m_Type));
    return ObjectHandle(m_Data.m_Handle);
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
    case eType_Texture2D: result = "Texture2D"; break;
    case eType_TextureCube: result = "TextureCube"; break;
    default: LOG(LogType_ProgrammerAssert, "Unknown MaterialParameter::EType %d when converting to string!", type);
    }
    return result;
}

} } //end namespace
