#include "HappySandBoxPCH.h"
#include "EntityPropertyDefaultFeel.h"
#include "ui_EntityPropertyDefaultFeel.h"

namespace hs {

EntityPropertyDefaultFeel::EntityPropertyDefaultFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertyDefaultFeel)
{
    m_UI->setupUi(this);

    connect(m_UI->m_TextBox, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
    connect(m_UI->m_TextBox, SIGNAL(textEdited(QString)), this, SLOT(onTextModified(QString)));
}

EntityPropertyDefaultFeel::~EntityPropertyDefaultFeel()
{
    delete m_UI;
}

void EntityPropertyDefaultFeel::setValue( const he::String& value )
{
    m_UI->m_TextBox->setText(value.c_str());
    setDirty(false);
}

void EntityPropertyDefaultFeel::setValueMixed()
{
    m_UI->m_TextBox->setPlaceholderText("<Mixed Values>");
    m_UI->m_TextBox->setText("");
    setDirty(false);
}

void EntityPropertyDefaultFeel::onEditingFinished()
{
    he::String value(m_UI->m_TextBox->text().toUtf8());
    ValueChanged(value);
}

void EntityPropertyDefaultFeel::onTextModified( const QString& /*text*/ )
{
    setDirty(true);
}

void EntityPropertyDefaultFeel::onDirtyChanged( const bool newDirty )
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