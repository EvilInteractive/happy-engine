#include "HappySandBoxPCH.h"
#include "EntityPropertyList.h"
#include "ui_EntityPropertyList.h"

#include "EntityPropertyListItem.h"

#include "EntityPropertyDefaultFeel.h"
#include "EntityPropertyDropDownFeel.h"
#include "EntityPropertyColorFeel.h"
#include "EntityPropertySliderFeel.h"

#include <Property.h>
#include <PropertyFeel.h>
#include <EntityComponentDesc.h>

#include <QTableWidgetItem>
#include <QObject>
#include <QLineEdit>

namespace hs {
EntityPropertyList::EntityPropertyList(QWidget *parent) :
    QWidget(parent),
    m_Component(he::HEFS::str),
    m_UI(NEW Ui::EntityPropertyList),
    m_CurrentRow(0)
{
    m_UI->setupUi(this);
}

EntityPropertyList::~EntityPropertyList()
{
    delete m_UI;
}

void EntityPropertyList::clear()
{
    m_UI->m_Table->clearContents();
    m_UI->m_Table->setRowCount(0);
    m_CurrentRow = 0;
    m_Items.clear();
}

void EntityPropertyList::addProperty(const he::ge::PropertyDesc& prop)
{
    QTableWidgetItem* name(nullptr);
    EntityPropertyListItem* item(nullptr);
    QWidget* widget(nullptr);

    getRowContents(m_Component, prop, name, item, widget);

    m_UI->m_Table->setRowCount(m_CurrentRow+1);
    m_UI->m_Table->setItem(m_CurrentRow, 0, name);
    m_UI->m_Table->setItem(m_CurrentRow, 1, item);
    m_UI->m_Table->setCellWidget(m_CurrentRow, 1, widget);

    m_Items[prop.m_Property->getName()] = item;
    he::eventCallback2<void, const he::FixedString&, he::ge::Property*> valueChangedCallback(
        [this](const he::FixedString& comp, he::ge::Property* prop){ ValueChanged(comp, prop); });
    item->ValueChanged += valueChangedCallback;

    ++m_CurrentRow;
}

void EntityPropertyList::setValue( const he::FixedString& prop, const he::String& value )
{
    hs::EntityPropertyListItem** item = m_Items.find(prop);
    HE_ASSERT(item, "Could not find property %s in list from component %s", prop.c_str(), m_Component.c_str());
    if (item)
    {
        (*item)->setValue(value);
    }
}

void EntityPropertyList::setValueMixed( const he::FixedString& prop )
{
    hs::EntityPropertyListItem** item = m_Items.find(prop);
    HE_ASSERT(item, "Could not find property %s in list from component %s", prop.c_str(), m_Component.c_str());
    if (item)
    {
        (*item)->setValueMixed();
    }
}

void EntityPropertyList::getRowContents( const he::FixedString& component, const he::ge::PropertyDesc& prop, QTableWidgetItem*& outName, hs::EntityPropertyListItem*& outItem, QWidget*& outWidget )
{
    outName = NEW QTableWidgetItem(QString(prop.m_DisplayName.c_str()));
    outName->setToolTip(QString(prop.m_Tooltip.c_str()));

    switch (prop.m_Feel->getType())
    {
    case he::ge::ePropertyFeel_DropDown: //dropdown
        {
            EntityPropertyDropDownFeel* feel(NEW EntityPropertyDropDownFeel(m_UI->m_Table));
            he::ge::PropertyFeelDropDown* dd(he::checked_cast<he::ge::PropertyFeelDropDown*>(prop.m_Feel));
            dd->getList().forEach([feel](const he::String& val)
            {
                feel->addValue(val);
            });
            outItem = NEW hs::EntityPropertyListItem(component, prop.m_Property->getName(), feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Slider: // slider
    case he::ge::ePropertyFeel_Default: //tb
        {
            EntityPropertyDefaultFeel* feel(NEW EntityPropertyDefaultFeel(m_UI->m_Table));
            outItem = NEW hs::EntityPropertyListItem(component, prop.m_Property->getName(), feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Color: //tb + colorpicker
        {
            EntityPropertyColorFeel* feel(NEW EntityPropertyColorFeel(m_UI->m_Table));
            outItem = NEW hs::EntityPropertyListItem(component, prop.m_Property->getName(), feel);
            outWidget = feel;
        } break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown property feel! %d", prop.m_Feel->getType());
    }
}

}
