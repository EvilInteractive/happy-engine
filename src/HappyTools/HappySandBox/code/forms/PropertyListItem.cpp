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
//Created: 2014/07/18
#include "HappySandBoxPCH.h" 
#include "PropertyListItem.h"

#include "Sandbox.h"
#include "system/EntityManager.h"
#include "PropertyFeel.h"

#include <EntityComponentDesc.h>
#include <PropertyFeel.h>
#include <PropertyConverter.h>
#include <Property.h>

#include <QLineEdit>

namespace hs {

    PropertyListItem::PropertyListItem(const he::ge::PropertyDesc& propDesc, PropertyFeel* uiFeel) 
        : QObject()
        , QTableWidgetItem(eEntityPropertyListItem_Default)
        , m_Desc(propDesc)
        , m_Feel(uiFeel)
    {
        he::eventCallback1<void, const he::String&> valueChangedCallback(std::bind(&PropertyListItem::applyNewValue, this, std::placeholders::_1));
        m_Feel->ValueChanged += valueChangedCallback;
        setValue(m_Desc.m_Converter->toString(m_Desc.m_Property));
    }

    PropertyListItem::~PropertyListItem()
    {
    }

    void PropertyListItem::applyNewValue(const he::String& value)
    {
        m_Desc.m_Converter->fromString(m_Desc.m_Property, value);
        ValueChanged(m_Desc.m_Property);
        setValue(m_Desc.m_Converter->toString(m_Desc.m_Property));
    }

    void PropertyListItem::setValue( const he::String& value )
    {
        m_Feel->setValue(value);
    }

    void PropertyListItem::setValueMixed()
    {
        m_Feel->setValueMixed();
    }

} //end namespace
