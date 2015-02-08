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
//Created: 2013/03/19
#include "HappyPCH.h" 

#include "EntityComponentDesc.h"

#include "Property.h"
#include "PropertyConverter.h"
#include "PropertyFeel.h"

namespace he {
namespace ge {

//////////////////////////////////////////////////////////////////////////
// EntityComponentDesc
//////////////////////////////////////////////////////////////////////////

EntityComponentDesc::EntityComponentDesc() : m_ID(), m_DisplayName("")
{

}

EntityComponentDesc::~EntityComponentDesc()
{
    m_Properties.forEach([](const he::FixedString& /*propID*/, PropertyDesc& desc)
    {
        HEDelete(desc.m_Property);
        desc.m_Property = nullptr;
        HEDelete(desc.m_Converter);
        desc.m_Converter = nullptr;
        HEDelete(desc.m_Feel);
        desc.m_Feel = nullptr;
    });
}

} } //end namespace
