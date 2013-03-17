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

#ifndef _HE_EntityComponentDesc_H_
#define _HE_EntityComponentDesc_H_
#pragma once

#include "EntityComponentType.h"

namespace he {
namespace ge {
class IEntityProperty;

enum PropertyFeel
{
    PropertyFeel_Default,
    PropertyFeel_Color,
    PropertyFeel_Slider,
    PropertyFeel_UpDown
};

class Property;
struct PropertyDesc
{
    Property* m_Property;
    he::String m_DisplayName;
    he::String m_Tooltip;
    PropertyFeel m_Feel;

    PropertyDesc(): m_Property(nullptr), m_DisplayName(""), m_Tooltip(""), m_Feel(PropertyFeel_Default) {}
    ~PropertyDesc() {}
    PropertyDesc(Property* const prop, const char* displayName, const char* tooltip = "", const PropertyFeel feel = PropertyFeel_Default)
        : m_Property(prop), m_DisplayName(displayName), m_Tooltip(tooltip), m_Feel(feel)
    {}

};

struct EntityComponentDesc
{
public:
    EntityComponentDesc()
        : m_ID(), m_DisplayName("")
    {
    }
    EntityComponentDesc(const EntityComponentDesc& other)
        : m_ID(other.m_ID), m_DisplayName(other.m_DisplayName)
    {
        m_Properties.append(other.m_Properties);
    }
    EntityComponentDesc& operator=(const EntityComponentDesc& other)
    {
        m_ID = other.m_ID;
        m_DisplayName = other.m_DisplayName;
        m_Properties.clear();
        m_Properties.append(other.m_Properties);
        return *this;
    }

    EntityComponentID m_ID;
    he::String m_DisplayName;
    he::ObjectList<PropertyDesc> m_Properties;
};

} } //end namespace

#endif
