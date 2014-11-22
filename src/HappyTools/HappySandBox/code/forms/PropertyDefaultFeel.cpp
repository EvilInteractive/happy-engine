#include "HappySandBoxPCH.h"
#include "PropertyDefaultFeel.h"
#include "ui_PropertyDefaultFeel.h"

namespace hs {

PropertyDefaultFeel::PropertyDefaultFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::PropertyDefaultFeel)
{
    m_UI->setupUi(this);

    connect(m_UI->m_TextBox, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
    connect(m_UI->m_TextBox, SIGNAL(textEdited(QString)), this, SLOT(onTextModified(QString)));
}

PropertyDefaultFeel::~PropertyDefaultFeel()
{
    delete m_UI;
}

void PropertyDefaultFeel::setValue( const he::String& value )
{
    m_UI->m_TextBox->setText(value.c_str());
    setDirty(false);
}

void PropertyDefaultFeel::setValueMixed()
{
    m_UI->m_TextBox->setPlaceholderText("<Mixed Values>");
    m_UI->m_TextBox->setText("");
    setDirty(false);
}

void PropertyDefaultFeel::onEditingFinished()
{
    he::String value(m_UI->m_TextBox->text().toUtf8());
    ValueChanged(value);
}

void PropertyDefaultFeel::onTextModified( const QString& /*text*/ )
{
    setDirty(true);
}

void PropertyDefaultFeel::onDirtyChanged( const bool newDirty )
{
    if (newDirty)
    {
        m_UI->m_TextBox->setStyleSheet(
            "border: 2px solid green;");
    }
    else
    {
        m_UI->m_TextBox->setStyleSheet("");
    }
}

}