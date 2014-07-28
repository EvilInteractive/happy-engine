#include "HappySandBoxPCH.h"
#include "EntityPropertyDropDownFeel.h"
#include "ui_EntityPropertyDropDownFeel.h"

namespace hs {

EntityPropertyDropDownFeel::EntityPropertyDropDownFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertyDropDownFeel)
{
    m_UI->setupUi(this);
    connect(m_UI->m_DropDown, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onSelectionChanged(const QString&)));
}

EntityPropertyDropDownFeel::~EntityPropertyDropDownFeel()
{
    delete m_UI;
}

void EntityPropertyDropDownFeel::setValue(const he::String& value)
{
    const int index(m_UI->m_DropDown->findText(QString(value.c_str())));
    HE_ASSERT(index >= 0, "Could not find a valid index for %s in the combobox!", value.c_str());
    m_UI->m_DropDown->setCurrentIndex(index);
}

void EntityPropertyDropDownFeel::setValueMixed()
{
    m_UI->m_DropDown->setCurrentIndex(-1);
    m_UI->m_DropDown->setCurrentText("<Mixed Values>");
}

void EntityPropertyDropDownFeel::onDirtyChanged(const bool /*newDirty*/)
{

}

void EntityPropertyDropDownFeel::addValue(const he::String& value)
{
    m_UI->m_DropDown->addItem(QString(value.c_str()));
}

void EntityPropertyDropDownFeel::onSelectionChanged(const QString& text)
{
    he::String newValue(text.toUtf8());
    ValueChanged(newValue);
}

}
