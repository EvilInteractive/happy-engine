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
//Created: 2013/03/19
#include "HappyPCH.h" 

#include "EntityComponentDesc.h"

#include "Property.h"

namespace he {
namespace ge {

//////////////////////////////////////////////////////////////////////////
// PropertyDesc
//////////////////////////////////////////////////////////////////////////
PropertyDesc::PropertyDesc() : m_Property(nullptr), m_DisplayName(""), m_Tooltip(""), m_Feel(PropertyFeel_Default)
{

}

PropertyDesc::PropertyDesc( Property* const prop, const char* displayName, const char* tooltip /*= ""*/, const PropertyFeel feel /*= PropertyFeel_Default*/ ) : m_Property(prop), m_DisplayName(displayName), m_Tooltip(tooltip), m_Feel(feel)
{

}

PropertyDesc::PropertyDesc( PropertyDesc&& other ) : m_Property(other.m_Property), m_DisplayName(std::move(other.m_DisplayName)), 
    m_Tooltip(std::move(other.m_Tooltip)), m_Feel(other.m_Feel)
{
    other.m_Property = nullptr;
}

PropertyDesc& PropertyDesc::operator=( PropertyDesc&& other )
{
    m_Property = other.m_Property;
    other.m_Property = nullptr;
    std::swap(m_DisplayName, other.m_DisplayName);
    std::swap(m_Tooltip, other.m_Tooltip);
    m_Feel = other.m_Feel;
    return *this;
}

PropertyDesc::~PropertyDesc()
{

}

//////////////////////////////////////////////////////////////////////////
// EntityComponentDesc
//////////////////////////////////////////////////////////////////////////

EntityComponentDesc::EntityComponentDesc() : m_ID(), m_DisplayName("")
{

}

EntityComponentDesc::~EntityComponentDesc()
{
    m_Properties.forEach([](PropertyDesc& desc)
    {
        delete desc.m_Property;
        desc.m_Property = nullptr;
    });
}

} } //end namespace
