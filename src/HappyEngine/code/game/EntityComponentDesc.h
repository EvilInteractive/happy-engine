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

namespace he {
namespace ge {

class Property;
class PropertyConverter;
class PropertyFeel;
struct HAPPY_ENTRY PropertyDesc
{
    Property* m_Property;
    he::String m_DisplayName;
    he::String m_Tooltip;
    PropertyConverter* m_Converter;
    PropertyFeel* m_Feel;

    PropertyDesc();
    PropertyDesc(Property* const prop, const char* displayName, const char* tooltip, 
        PropertyConverter* const converter, PropertyFeel* const feel);
    ~PropertyDesc();
    PropertyDesc(PropertyDesc&& other);
    PropertyDesc& operator=(PropertyDesc&& other);

private:
    PropertyDesc(const PropertyDesc& desc);
    PropertyDesc& operator=(const PropertyDesc&);
};

struct HAPPY_ENTRY EntityComponentDesc
{
public:
    EntityComponentDesc();
    ~EntityComponentDesc();

    he::FixedString m_ID;
    he::String m_DisplayName;
    he::ObjectList<PropertyDesc> m_Properties;

private:
    EntityComponentDesc(const EntityComponentDesc&);
    EntityComponentDesc& operator=(const EntityComponentDesc&);
};

} } //end namespace

#endif
