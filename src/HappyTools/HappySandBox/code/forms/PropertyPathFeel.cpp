#include "HappySandBoxPCH.h"
#include "PropertyPathFeel.h"
#include "ui_PropertyPathFeel.h"

#include <QFileDialog>

namespace hs {

PropertyPathFeel::PropertyPathFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(HENew(Ui::PropertyPathFeel))
{
    m_UI->setupUi(this);

    connect(m_UI->m_TextBox, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
    connect(m_UI->m_TextBox, SIGNAL(textEdited(QString)), this, SLOT(onTextModified(QString)));
    connect(m_UI->m_PathDialog, SIGNAL(clicked()), this, SLOT(onOpenPathDialog()));
}

PropertyPathFeel::~PropertyPathFeel()
{
    HEDelete(m_UI);
}

void PropertyPathFeel::setValue( const he::String& value )
{
    m_UI->m_TextBox->setText(value.c_str());
    setDirty(false);
}

void PropertyPathFeel::setValueMixed()
{
    m_UI->m_TextBox->setPlaceholderText("<Mixed Values>");
    m_UI->m_TextBox->setText("");
    setDirty(false);
}

void PropertyPathFeel::onEditingFinished()
{
    he::String value(m_UI->m_TextBox->text().toUtf8());
    ValueChanged(value);
}

void PropertyPathFeel::onTextModified( const QString& /*text*/ )
{
    setDirty(true);
}

void PropertyPathFeel::onDirtyChanged( const bool newDirty )
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

void PropertyPathFeel::onOpenPathDialog()
{
    QString fileName;
    if (m_IsDirectory)
    {
        fileName = QFileDialog::getExistingDirectory(this, "Select Directory", m_UI->m_TextBox->text());
    }
    else
    {
        fileName = QFileDialog::getOpenFileName(this, "Select File", m_UI->m_TextBox->text(), m_Filter.c_str());
    }
    if (!fileName.isNull() && fileName != m_UI->m_TextBox->text())
    {
        setDirty(true);
        m_UI->m_TextBox->setText(fileName);
        he::String value(fileName.toUtf8());
        ValueChanged(value);
    }
}

void PropertyPathFeel::setOpenFile( const he::String& filter )
{
    m_IsDirectory = false;
    m_Filter = filter;
}

void PropertyPathFeel::setOpenDirectory()
{
    m_IsDirectory = true;
}

}
