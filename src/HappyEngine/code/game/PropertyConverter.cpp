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
//Created: 2013/03/20
#include "HappyPCH.h" 

#include "PropertyConverter.h"

namespace he {
namespace ge {

void PropertyConverterVec2::fromString( Property* const prop, const he::String& str )
{
    const char* charBuf(str.c_str());
    vec2 result(0, 0);
    if (charBuf != nullptr)
    {
        he::uint8 component(0);
        for (; *charBuf != '\0';)
        {
            if (component == 0)
            {
                char* nextVal(nullptr);
                result.x = static_cast<float>(std::strtod(charBuf, &nextVal));
                charBuf = nextVal;
                ++component;
            }
            else
            {
                if (*charBuf == ',')
                {
                    ++charBuf;
                    result.y = static_cast<float>(std::strtod(charBuf, nullptr));
                    break;
                }
                else
                {
                    ++charBuf;
                }
            }
        }
    }
    prop->set<vec2>(result);
}

he::String PropertyConverterVec2::toString( const Property* const prop )
{
    char buf[40];
    const vec2& value(prop->get<vec2>());
    hesnprintf(buf, 39, "%.*f, %.*f", m_Precision, value.x, m_Precision, value.y);
    return he::String(buf);
}


void PropertyConverterVec3::fromString( Property* const prop, const he::String& str )
{
    const char* charBuf(str.c_str());
    vec3 result(0, 0, 0);
    if (charBuf != nullptr)
    {
        he::uint8 component(0);
        for (; *charBuf != '\0';)
        {
            if (component == 0)
            {
                char* nextVal(nullptr);
                result.x = static_cast<float>(std::strtod(charBuf, &nextVal));
                charBuf = nextVal;
                ++component;
            }
            else
            {
                if (*charBuf == ',')
                {
                    ++charBuf;
                    char* nextVal(nullptr);
                    const float val(static_cast<float>(std::strtod(charBuf, &nextVal)));
                    if (component == 1) 
                    {
                        result.y = val;
                    }
                    else 
                    { 
                        result.z = val; 
                        break; 
                    }
                    charBuf = nextVal;
                    ++component;
                }
                else
                {
                    ++charBuf;
                }
            }
        }
    }
    prop->set<vec3>(result);
}

he::String PropertyConverterVec3::toString( const Property* const prop )
{
    char buf[60];
    const vec3& value(prop->get<vec3>());
    hesnprintf(buf, 59, "%.*f, %.*f, %.*f", m_Precision, value.x, m_Precision, value.y, m_Precision, value.z);
    return he::String(buf);
}


void PropertyConverterVec4::fromString( Property* const prop, const he::String& str )
{
    const char* charBuf(str.c_str());
    vec4 result(0, 0, 0, 0);
    if (charBuf != nullptr)
    {
        he::uint8 component(0);
        for (; *charBuf != '\0';)
        {
            if (component == 0)
            {
                char* nextVal(nullptr);
                result.x = static_cast<float>(std::strtod(charBuf, &nextVal));
                charBuf = nextVal;
                ++component;
            }
            else
            {
                if (*charBuf == ',')
                {
                    ++charBuf;
                    char* nextVal(nullptr);
                    const float val(static_cast<float>(std::strtod(charBuf, &nextVal)));
                    if (component == 1) 
                    {
                        result.y = val;
                    }
                    else if (component == 2)
                    { 
                        result.z = val; 
                    }
                    else
                    { 
                        result.w = val; 
                        break;
                    }
                    charBuf = nextVal;
                    ++component;
                }
                else
                {
                    ++charBuf;
                }
            }
        }
    }
    prop->set<vec4>(result);
}

he::String PropertyConverterVec4::toString( const Property* const prop )
{
    char buf[80];
    const vec4& value(prop->get<vec4>());
    hesnprintf(buf, 79, "%.*f, %.*f, %.*f, %.*f", m_Precision, value.x, m_Precision, value.y, m_Precision, value.z, m_Precision, value.w);
    return he::String(buf);
}

} } //end namespace
