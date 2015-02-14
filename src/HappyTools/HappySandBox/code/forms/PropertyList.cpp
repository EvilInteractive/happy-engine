#include "HappySandBoxPCH.h"
#include "PropertyList.h"
#include "ui_PropertyList.h"

#include "PropertyListItem.h"

#include "PropertyDefaultFeel.h"
#include "PropertyDropDownFeel.h"
#include "PropertyColorFeel.h"
#include "PropertySliderFeel.h"
#include "PropertyCheckBoxFeel.h"
#include "PropertyPathFeel.h"

#include <Property.h>
#include <PropertyFeel.h>
#include <EntityComponentDesc.h>

#include <QTableWidgetItem>
#include <QObject>
#include <QLineEdit>

namespace hs {
PropertyList::PropertyList(QWidget *parent) :
    QWidget(parent),
    m_UI(HENew(Ui::PropertyList)),
    m_CurrentRow(0)
{
    m_UI->setupUi(this);
    m_UI->m_Table->horizontalHeader()->setMaximumHeight(20);
    QFont font;
    font.setPointSize(6);
    m_UI->m_Table->horizontalHeader()->setFont(font);
}

PropertyList::~PropertyList()
{
    HEDelete(m_UI);
}

void PropertyList::clear()
{
    m_UI->m_Table->clearContents();
    m_UI->m_Table->setRowCount(0);
    m_CurrentRow = 0;
    m_Items.clear();
}

void PropertyList::addProperty(const he::ge::PropertyDesc& prop)
{
    QTableWidgetItem* name(nullptr);
    PropertyListItem* item(nullptr);
    QWidget* widget(nullptr);

    getRowContents(prop, name, item, widget);

    m_UI->m_Table->setRowCount(m_CurrentRow+1);
    m_UI->m_Table->setItem(m_CurrentRow, 0, name);
    m_UI->m_Table->setItem(m_CurrentRow, 1, item);
    m_UI->m_Table->setCellWidget(m_CurrentRow, 1, widget);

    m_Items[prop.m_Property->getName()] = item;
    he::eventCallback1<void, he::ge::Property*> valueChangedCallback([this](he::ge::Property* prop){ ValueChanged(prop); });
    item->ValueChanged += valueChangedCallback;

    ++m_CurrentRow;

    int h(0);
    h += m_UI->m_Table->contentsMargins().top();
    h += m_UI->m_Table->contentsMargins().bottom();
    h += m_UI->m_Table->horizontalHeader()->height();
    for (int i(0); i < m_UI->m_Table->rowCount(); ++i)
        h += m_UI->m_Table->rowHeight(i);
    m_UI->m_Table->setMinimumHeight(h);
    m_UI->m_Table->setMaximumHeight(h);
    setMinimumHeight(h + 2);
    setMaximumHeight(h + 2);
}

void PropertyList::setValue( const he::FixedString& prop, const he::String& value )
{
    hs::PropertyListItem** item = m_Items.find(prop);
    HE_ASSERT(item, "Could not find property %s in list", prop.c_str());
    if (item)
    {
        (*item)->setValue(value);
    }
}

void PropertyList::setValueMixed( const he::FixedString& prop )
{
    hs::PropertyListItem** item = m_Items.find(prop);
    HE_ASSERT(item, "Could not find property %s in list from component %s", prop.c_str());
    if (item)
    {
        (*item)->setValueMixed();
    }
}

void PropertyList::getRowContents( const he::ge::PropertyDesc& prop, QTableWidgetItem*& outName, hs::PropertyListItem*& outItem, QWidget*& outWidget )
{
    outName = HENew(QTableWidgetItem)(QString(prop.m_DisplayName.c_str()));
    outName->setToolTip(QString(prop.m_Tooltip.c_str()));

    switch (prop.m_Feel->getType())
    {
    case he::ge::ePropertyFeel_DropDown: //dropdown
        {
            PropertyDropDownFeel* feel(HENew(PropertyDropDownFeel)(m_UI->m_Table));
            he::ge::PropertyFeelDropDown* dd(he::checked_cast<he::ge::PropertyFeelDropDown*>(prop.m_Feel));
            dd->getList().forEach([feel](const he::String& val)
            {
                feel->addValue(val);
            });
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Slider: // slider
        {
            PropertySliderFeel* feel(HENew(PropertySliderFeel)(m_UI->m_Table));
            he::ge::PropertyFeelSlider* slider(he::checked_cast<he::ge::PropertyFeelSlider*>(prop.m_Feel));
            feel->setRange(slider->getMin(), slider->getMax());
            feel->setTicksInterval((slider->getMax() - slider->getMin()) / 10.0f);
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Default: //tb
        {
            PropertyDefaultFeel* feel(HENew(PropertyDefaultFeel)(m_UI->m_Table));
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Path: //tb + button
        {
            PropertyPathFeel* feel(HENew(PropertyPathFeel)(m_UI->m_Table));
            he::ge::PropertyFeelPath* pathFeel(he::checked_cast<he::ge::PropertyFeelPath*>(prop.m_Feel));
            if (pathFeel->isDirectory())
                feel->setOpenDirectory();
            else
                feel->setOpenFile(pathFeel->getFilter());
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_Color: //tb + colorpicker
        {
            PropertyColorFeel* feel(HENew(PropertyColorFeel)(m_UI->m_Table));
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    case he::ge::ePropertyFeel_CheckBox: //checkbox
        {
            PropertyCheckBoxFeel* feel(HENew(PropertyCheckBoxFeel)(m_UI->m_Table));
            outItem = HENew(hs::PropertyListItem)(prop, feel);
            outWidget = feel;
        } break;
    default: LOG(he::LogType_ProgrammerAssert, "Unknown property feel! %d", prop.m_Feel->getType());
    }
}

}
