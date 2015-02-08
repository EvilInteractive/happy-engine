#include "HappySandBoxPCH.h"
#include "PropertyCheckBoxFeel.h"
#include "ui_PropertyCheckBoxFeel.h"

#include <PropertyConverter.h>

namespace hs {

PropertyCheckBoxFeel::PropertyCheckBoxFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(HENew(Ui::PropertyCheckBoxFeel))
{
    m_UI->setupUi(this);

    connect(m_UI->m_CheckBox, SIGNAL(stateChanged(state)), this, SLOT(onCheckedChanged(state)));
}

PropertyCheckBoxFeel::~PropertyCheckBoxFeel()
{
    HEDelete(m_UI);
}

void PropertyCheckBoxFeel::setValue( const he::String& value )
{
    m_UI->m_CheckBox->setCheckState(he::ge::PropertyConverterBool::fromString(value)? Qt::Checked : Qt::Unchecked);
    setDirty(false);
}

void PropertyCheckBoxFeel::setValueMixed()
{
    m_UI->m_CheckBox->setCheckState(Qt::PartiallyChecked);
    setDirty(false);
}

void PropertyCheckBoxFeel::onCheckedChanged( const int newState )
{
    he::String value(he::ge::PropertyConverterBool::toString(newState == Qt::Checked));
    ValueChanged(value);
}

void PropertyCheckBoxFeel::onDirtyChanged( const bool newDirty )
{
    if (newDirty)
    {
        m_UI->m_CheckBox->setStyleSheet(
            "border: 2px solid green;");
    }
    else
    {
        m_UI->m_CheckBox->setStyleSheet("");
    }
}

}