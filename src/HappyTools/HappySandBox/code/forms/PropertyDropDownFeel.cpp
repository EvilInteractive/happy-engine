#include "HappySandBoxPCH.h"
#include "PropertyDropDownFeel.h"
#include "ui_PropertyDropDownFeel.h"

namespace hs {

PropertyDropDownFeel::PropertyDropDownFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(HENew(Ui::PropertyDropDownFeel)),
    m_HoldEvents(false)
{
    m_UI->setupUi(this);
    connect(m_UI->m_DropDown, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onSelectionChanged(const QString&)));
}

PropertyDropDownFeel::~PropertyDropDownFeel()
{
    HEDelete(m_UI);
}

void PropertyDropDownFeel::setValue(const he::String& value)
{
    m_HoldEvents = true;
    const int index(m_UI->m_DropDown->findText(QString(value.c_str())));
    HE_ASSERT(index >= 0, "Could not find a valid index for %s in the combobox!", value.c_str());
    m_UI->m_DropDown->setCurrentIndex(index);
    m_HoldEvents = false;
}

void PropertyDropDownFeel::setValueMixed()
{
    m_HoldEvents = true;
    m_UI->m_DropDown->setCurrentIndex(-1);
    m_HoldEvents = false;
}

void PropertyDropDownFeel::onDirtyChanged(const bool /*newDirty*/)
{

}

void PropertyDropDownFeel::addValue(const he::String& value)
{
    m_UI->m_DropDown->addItem(QString(value.c_str()));
}

void PropertyDropDownFeel::onSelectionChanged(const QString& text)
{
    if (!m_HoldEvents)
    {
        he::String newValue(text.toUtf8());
        ValueChanged(newValue);
    }
}

}
