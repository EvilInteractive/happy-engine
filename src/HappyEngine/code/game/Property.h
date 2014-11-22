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

    virtual IPropertyValue* clone() const = 0;
    virtual bool equals(const IPropertyValue* const value) const = 0;
};

template<typename T>
class PropertyValue : public IPropertyValue
{
public:
    explicit PropertyValue(const T& defaultValue): m_Value(defaultValue)  {}
    virtual ~PropertyValue() {}

    IPropertyValue* clone() const override { return NEW PropertyValue<T>(m_Value); }
    bool equals(const IPropertyValue* const value) const override
    {
        return checked_cast<const PropertyValue<T>*>(value)->get() == get();
    }

    void set(const T& value) { m_Value = value; }
    const T& get() const { return m_Value; }

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
    Property(Property&& other)
        : m_Name(other.m_Name)
        , m_Value(other.m_Value)
    {
        other.m_Name = HEFS::str;
        other.m_Value = nullptr;
    }
    Property& operator=(Property&& other)
    {
        std::swap(other.m_Name, m_Name);
        std::swap(other.m_Value, m_Value);
        return *this;
    }

    Property* clone() const
    {
        Property* newProp(NEW Property);
        newProp->m_Name = m_Name;
        newProp->m_Value = m_Value->clone();
        return newProp;
    }

    template<typename T>
    void init(const he::FixedString& name, const T& defaultValue)
    {
        HE_ASSERT(m_Value == nullptr, "Property %s already initialize", m_Name.c_str());
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
        const PropertyValue<T>* prop(checked_cast<const PropertyValue<T>*>(m_Value));
        return prop->get();
    }

    const bool equals(const Property* const other) const
    {
        return m_Name == other->m_Name && m_Value->equals(other->m_Value);
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
