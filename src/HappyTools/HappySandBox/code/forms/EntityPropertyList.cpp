#include "HappySandBoxPCH.h"

#include "EntityPropertyList.h"
#include "ui_EntityPropertyList.h"

#include <EntityComponentDesc.h>
#include <PropertyFeel.h>

#include <QTableWidgetItem>
#include <QPlainTextEdit>

enum ETableItemType
{
    eTableItemType_Default = QTableWidgetItem::UserType,
    eTableItemType_Color,
    eTableItemType_Slider,
    eTableItemType_DropDown,
};

class DefaultFeelTableItem : public QTableWidgetItem
{
public:
    DefaultFeelTableItem() : QTableWidgetItem(eTableItemType_Default)
    {
    }
};

EntityPropertyList::EntityPropertyList(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertyList),
    m_CurrentRow(0)
{
    m_UI->setupUi(this);
}

EntityPropertyList::~EntityPropertyList()
{
    delete m_UI;
}

namespace
{
    void GetRowContents(const he::ge::PropertyDesc& prop, QTableWidgetItem*& outName, QTableWidgetItem*& outItem, QWidget*& outWidget )
    {
        outName = NEW QTableWidgetItem(QString(prop.m_DisplayName.c_str()));
        outItem = NEW DefaultFeelTableItem();
        switch (prop.m_Feel->getType())
        {
            case he::ge::ePropertyFeel_Default:
            {
                outWidget = NEW QPlainTextEdit();
            } break; //tb
            case he::ge::ePropertyFeel_Color:
            {
                outWidget = NEW QPlainTextEdit();
            }break; //tb + colorpicker
            case he::ge::ePropertyFeel_Slider:
            {
                outWidget = NEW QPlainTextEdit();
            } break; // slider
            case he::ge::ePropertyFeel_DropDown:
            {
                outWidget = NEW QPlainTextEdit();
            } break; //dropdown
            default: LOG(he::LogType_ProgrammerAssert, "Unknown property feel! %d", prop.m_Feel->getType());
        }
    }
}

void EntityPropertyList::AddProperty(const he::ge::PropertyDesc& prop)
{
    QTableWidgetItem* name(nullptr);
    QTableWidgetItem* item(nullptr);
    QWidget* widget(nullptr);

    GetRowContents(prop, name, item, widget);

    m_UI->m_Table->setRowCount(m_CurrentRow+1);
    m_UI->m_Table->setItem(m_CurrentRow, 0, name);
    m_UI->m_Table->setItem(m_CurrentRow, 1, item);
    m_UI->m_Table->setCellWidget(m_CurrentRow, 1, widget);

    ++m_CurrentRow;
}
