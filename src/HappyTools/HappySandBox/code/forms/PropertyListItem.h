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

#ifndef _HE_EntityPropertyListItem_H_
#define _HE_EntityPropertyListItem_H_
#pragma once

#include <QTableWidget>
#include <PropertyDesc.h>

namespace he {
namespace ge {
    class Property;
} }

namespace hs {

class PropertyFeel;

enum EEntityPropertyListItem
{
    eEntityPropertyListItem_Default = QTableWidgetItem::UserType,
    eEntityPropertyListItem_Color,
    eEntityPropertyListItem_Slider,
    eEntityPropertyListItem_DropDown,
};

class PropertyListItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT
public:
    explicit PropertyListItem(const he::ge::PropertyDesc& propDesc, PropertyFeel* uiFeel);
    virtual ~PropertyListItem();
    
    virtual void setValue(const he::String& value);
    virtual void setValueMixed();
    
    he::event1<void, he::ge::Property*> ValueChanged;

protected:
    void applyNewValue(const he::String& newValue);

private:
    he::ge::PropertyDesc m_Desc;
    PropertyFeel* m_Feel;
};

} //end namespace

#endif
