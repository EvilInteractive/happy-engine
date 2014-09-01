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

#ifndef _HE_PropertyConverter_H_
#define _HE_PropertyConverter_H_
#pragma once

#include "Property.h"

namespace he {
namespace ge {
class Property;
class HAPPY_ENTRY PropertyConverter
{
public:
    virtual ~PropertyConverter() {}
    virtual void fromString(Property* const prop, const he::String& str) = 0;
    virtual he::String toString(const Property* const prop) = 0;
};

class PropertyConverterString : public PropertyConverter
{
public:
    virtual ~PropertyConverterString() {}
    virtual void fromString(Property* const prop, const he::String& str)
    {
        prop->set<he::String>(str);
    }
    virtual he::String toString(const Property* const prop)
    {
        return prop->get<he::String>();
    }
};

class PropertyConverterInt : public PropertyConverter
{
public:
    virtual ~PropertyConverterInt() {}
    static int fromString(const he::String& str)
    {
        return checked_numcast<int>(atol(str.c_str()));
    }
    static he::String toString(const int value)
    {
        char buf[16];
        hesnprintf(buf, 15, "%d", value);
        return he::String(buf);
    }

    virtual void fromString(Property* const prop, const he::String& str)
    {
        return prop->set<int>(fromString(str));
    }
    virtual he::String toString(const Property* const prop)
    {
        return toString(prop->get<int>());
    }
};

class HAPPY_ENTRY PropertyConverterFloat : public PropertyConverter
{
public:
    explicit PropertyConverterFloat(const uint8 precision = 2): m_Precision(precision) {}
    virtual ~PropertyConverterFloat() {}
    virtual void fromString(Property* const prop, const he::String& str);
    virtual he::String toString(const Property* const prop);

    static float fromString(const he::String& str);
    static he::String toString(const float val, const int decimals);

private:
    uint8 m_Precision;
};

class HAPPY_ENTRY PropertyConverterVec2 : public PropertyConverter
{
public:
    explicit PropertyConverterVec2(const uint8 precision = 4): m_Precision(precision) {}
    virtual ~PropertyConverterVec2() {}
    virtual void fromString(Property* const prop, const he::String& str);
    virtual he::String toString(const Property* const prop);

    static he::vec2 fromString(const he::String& str);
    static he::String toString(const he::vec2& vec, const uint8 precision = 4);
private:
    uint8 m_Precision;
};

class HAPPY_ENTRY PropertyConverterVec3 : public PropertyConverter
{
public:
    explicit PropertyConverterVec3(const uint8 precision = 4): m_Precision(precision) {}
    virtual ~PropertyConverterVec3() {}
    virtual void fromString(Property* const prop, const he::String& str);
    virtual he::String toString(const Property* const prop);

    static he::vec3 fromString(const he::String& str);
    static he::String toString(const he::vec3& vec, const uint8 precision = 4);
private:
    uint8 m_Precision;
};

class HAPPY_ENTRY PropertyConverterVec4 : public PropertyConverter
{
public:
    explicit PropertyConverterVec4(const uint8 precision = 4): m_Precision(precision) {}
    virtual ~PropertyConverterVec4() {}
    virtual void fromString(Property* const prop, const he::String& str);
    virtual he::String toString(const Property* const prop);

    static he::vec4 fromString(const he::String& str);
    static he::String toString(const he::vec4& vec, const uint8 precision = 4);

private:
    uint8 m_Precision;
};

template<typename T>
class PropertyConverterEnum : public PropertyConverter
{
public:
    typedef T (*StringToEnum)(const char* const str);
    typedef const char* (*EnumToString)(const T value);
    PropertyConverterEnum(const StringToEnum stringToEnum, const EnumToString enumToString)
        : f_StringToEnum(stringToEnum)
        , f_EnumToString(enumToString) 
    {}
    virtual ~PropertyConverterEnum() {}
    virtual void fromString(Property* const prop, const he::String& str)
    {
        prop->set<T>(f_StringToEnum(str.c_str()));
    }
    virtual he::String toString(const Property* const prop)
    {
        return f_EnumToString(prop->get<T>());
    }

private:
    StringToEnum f_StringToEnum;
    EnumToString f_EnumToString;
};

} } //end namespace

#endif
