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
#include "EntityPropertyListItem.h"

#include "Sandbox.h"
#include "system/EntityManager.h"
#include "EntityPropertyFeel.h"

#include <EntityComponentDesc.h>
#include <PropertyFeel.h>
#include <PropertyConverter.h>
#include <Property.h>

#include <QLineEdit>

namespace hs {

    EntityPropertyListItem::EntityPropertyListItem(const he::FixedString& component, const he::FixedString& prop, EntityPropertyFeel* const feel) 
        : QObject()
        , QTableWidgetItem(eEntityPropertyListItem_Default)
        , m_Component(component)
        , m_Property(prop)
        , m_Feel(feel)
    {
        he::eventCallback1<void, const he::String&> valueChangedCallback(std::bind(&EntityPropertyListItem::applyNewValue, this, std::placeholders::_1));
        m_Feel->ValueChanged += valueChangedCallback;
    }

    EntityPropertyListItem::~EntityPropertyListItem()
    {
    }

    void EntityPropertyListItem::applyNewValue(const he::String& value)
    {
        he::ge::EntityComponentDesc* desc(
            hs::Sandbox::getInstance()->getEntityManager()->getComponentDescriptor(m_Component));
        he::ge::PropertyDesc* propDesc(desc->m_Properties.find(m_Property));
        HE_ASSERT(propDesc, "Could not find propertydesc %s for component %s", m_Component.c_str(), m_Property.c_str());
        if (propDesc)
        {
            propDesc->m_Converter->fromString(propDesc->m_Property, value);
            ValueChanged(m_Component, propDesc->m_Property);
            setValue(propDesc->m_Converter->toString(propDesc->m_Property));
        }
    }

    void EntityPropertyListItem::setValue( const he::String& value )
    {
        m_Feel->setValue(value);
    }

    void EntityPropertyListItem::setValueMixed()
    {
        m_Feel->setValueMixed();
    }

} //end namespace
