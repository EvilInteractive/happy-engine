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
//Created: 10/03/2013

#ifndef _HE_EntityProperty_H_
#define _HE_EntityProperty_H_
#pragma once

namespace he {
namespace ge {
    
class IPropertyValue
{
public:
    virtual ~IPropertyValue() {}
};

template<typename T>
class PropertyValue : public IPropertyValue
{
public:
    explicit PropertyValue(const T& defaultValue): m_Value(defaultValue)  {}
    virtual ~PropertyValue() {}

    void set(const T& value) { m_Value = value; }
    const T& get() { return m_Value; }

private:
    T m_Value;
};

class Property
{
public:
    Property()
        : m_Name(HEFS::str), m_Value(nullptr)
    {
    }
    ~Property() 
    {
        delete m_Value;
    }

    template<typename T>
    void init(const he::FixedString& name, const T& defaultValue)
    {
        HE_ASSERT(m_Value == nullptr, "Property %s already initialize", m_Name);
        m_Name = name;
        m_Value = NEW PropertyValue<T>(defaultValue);
    }

    template<typename T>
    void set(const T& value) 
    { 
        PropertyValue<T>* prop(checked_cast<PropertyValue<T>*>(m_Value));
        prop->set(value);
    }

    template<typename T>
    const T& get() const
    { 
        PropertyValue<T>* prop(checked_cast<PropertyValue<T>*>(m_Value));
        return prop->get();
    }

    const he::FixedString& getName() const { return m_Name; }
        
private:
    Property(const Property&);
    Property& operator=(const Property&);

    he::FixedString m_Name;
    IPropertyValue* m_Value;
};

} } //end namespace

#endif
