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

template<typename T>
class PropertyConverterInt : public PropertyConverter
{
public:
    virtual ~PropertyConverterInt() {}
    virtual void fromString(Property* const prop, const he::String& str)
    {
        prop->set<T>(checked_cast<T>(atol(str.c_str())));
    }
    virtual he::String toString(const Property* const prop)
    {
        return he::String(ltoa(checked_cast<long>(prop->get<T>())));
    }
};

class HAPPY_ENTRY PropertyConverterFloat : public PropertyConverter
{
public:
    explicit PropertyConverterFloat(const uint8 precision = 2): m_Precision(precision) {}
    virtual ~PropertyConverterFloat() {}
    virtual void fromString(Property* const prop, const he::String& str)
    {
        prop->set<float>(static_cast<float>(atof(str.c_str())));
    }
    virtual he::String toString(const Property* const prop)
    {
        char buf[20];
        sprintf(buf, "%.*f", m_Precision, prop->get<float>());
        return he::String(buf);
    }

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
