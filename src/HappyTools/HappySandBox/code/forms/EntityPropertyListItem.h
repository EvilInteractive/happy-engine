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

namespace he {
namespace ge {
    class Property;
} }

namespace hs {

enum EEntityPropertyListItem
{
    eEntityPropertyListItem_Default = QTableWidgetItem::UserType,
    eEntityPropertyListItem_Color,
    eEntityPropertyListItem_Slider,
    eEntityPropertyListItem_DropDown,
};

class EntityPropertyListItem : public QObject, public QTableWidgetItem
{
    Q_OBJECT
public:
    EntityPropertyListItem(const he::FixedString& component, const he::FixedString& prop);
    virtual ~EntityPropertyListItem() {}

    virtual void setValue(const he::String& value) = 0;
    virtual void setValueMixed() = 0;

    bool isDirty() const { return m_IsDirty; }
    void setDirty(const bool dirty);

    he::event2<void, const he::FixedString& /*component*/, he::ge::Property*> ValueChanged;

protected:
    void applyNewValue(const he::String& newValue);
    virtual void onDirtyChanged(const bool /*newValue*/) {}

private:
    he::FixedString m_Component;
    he::FixedString m_Property;

    bool m_IsDirty;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class EntityPropertyListDefaultFeelItem : public EntityPropertyListItem
{
    Q_OBJECT
public:
    EntityPropertyListDefaultFeelItem(const he::FixedString& component, const he::FixedString& prop, QLineEdit* widget);

    virtual void setValue(const he::String& value);
    virtual void setValueMixed();

    private slots:
        void onEditingFinished();
        void onTextModified(const QString& text);

protected:
    void onDirtyChanged(const bool newValue);

private:
    void applyValueChange();

    QLineEdit* m_Textbox;
};

} //end namespace

#endif
