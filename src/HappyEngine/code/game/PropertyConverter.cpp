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
    prop->set<vec2>(fromString(str));
}

he::vec2 PropertyConverterVec2::fromString( const he::String& str )
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
    return result;
}

he::String PropertyConverterVec2::toString( const Property* const prop )
{
    return toString(prop->get<vec2>(), m_Precision);
}

he::String PropertyConverterVec2::toString( const vec2& value, const uint8 precision )
{
    char buf[40];
    hesnprintf(buf, 39, "%.*f, %.*f", precision, value.x, precision, value.y);
    return he::String(buf);
}


void PropertyConverterVec3::fromString( Property* const prop, const he::String& str )
{
    prop->set<vec3>(fromString(str));
}

vec3 PropertyConverterVec3::fromString( const he::String& str )
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
    return result;
}

he::String PropertyConverterVec3::toString( const Property* const prop )
{
    return toString(prop->get<vec3>());
}

he::String PropertyConverterVec3::toString( const vec3& value, const uint8 precision )
{
    char buf[60];
    hesnprintf(buf, 59, "%.*f, %.*f, %.*f", precision, value.x, precision, value.y, precision, value.z);
    return he::String(buf);
}


void PropertyConverterVec4::fromString( Property* const prop, const he::String& str )
{
    prop->set<vec4>(fromString(str));
}

he::vec4 PropertyConverterVec4::fromString( const he::String& str )
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
    return result;
}

he::String PropertyConverterVec4::toString( const Property* const prop )
{
    const vec4& value(prop->get<vec4>());
    return toString(value, m_Precision);
}

he::String PropertyConverterVec4::toString( const he::vec4& value, const uint8 precision/* = 4*/ )
{
    char buf[80];
    buf[79] = '\0';
    hesnprintf(buf, 79, "%.*f, %.*f, %.*f, %.*f", precision, value.x, precision, value.y, precision, value.z, precision, value.w);
    return he::String(buf);
}

void PropertyConverterFloat::fromString(Property* const prop, const he::String& str)
{
    return prop->set<float>(fromString(str));
}

he::String PropertyConverterFloat::toString(const Property* const prop)
{
    return toString(prop->get<float>(), m_Precision);
}

float PropertyConverterFloat::fromString(const he::String& str)
{
    return static_cast<float>(atof(str.c_str()));
}

he::String PropertyConverterFloat::toString(const float val, const int decimals)
{
    char buf[20];
    buf[19] = '\0';
    hesnprintf(buf, 19, "%.*f", decimals, val);
    return he::String(buf);
}


void PropertyConverterBool::fromString( Property* const prop, const he::String& str )
{
    return prop->set<bool>(fromString(str));
}

he::String PropertyConverterBool::toString( const Property* const prop )
{
    return toString(prop->get<bool>());
}

bool PropertyConverterBool::fromString( const he::String& str )
{
    if (stricmp(str.c_str(), "true") == 0)
        return true;
    return false;
}

he::String PropertyConverterBool::toString( const bool val )
{
    return val? "true" : "false";
}

} } //end namespace
