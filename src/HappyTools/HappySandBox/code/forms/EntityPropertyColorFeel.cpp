#include "HappySandBoxPCH.h"
#include "EntityPropertyColorFeel.h"
#include "ui_EntityPropertyColorFeel.h"

#include "Sandbox.h"

#include <PropertyConverter.h>

#include <QColorDialog>

namespace hs {

EntityPropertyColorFeel::EntityPropertyColorFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::EntityPropertyColorFeel)
{
    m_UI->setupUi(this);
    m_UI->m_ColorBox->installEventFilter(this);
}

EntityPropertyColorFeel::~EntityPropertyColorFeel()
{
    delete m_UI;
}

void EntityPropertyColorFeel::setValue( const he::String& value )
{
    m_UI->m_ColorBox->setText(value.c_str());
    UpdateBackground(value);
    setDirty(false);
}

void EntityPropertyColorFeel::setValueMixed()
{
    m_UI->m_ColorBox->setPlaceholderText("<Mixed Values>");
    m_UI->m_ColorBox->setText("");
    setDirty(false);
}

void EntityPropertyColorFeel::onColorChanged(const QColor& color)
{
    UpdateBackground(color);
    qreal r, g, b, a;
    color.getRgbF(&r, &g, &b, &a);
    char buff[32];
    buff[31] = '\0';
    he::hesnprintf(buff, 31, "%.4f, %.4f, %.4f, %.4f", r, g, b, a);
    he::String value(buff);
    ValueChanged(value);
}

void EntityPropertyColorFeel::onColorApplied()
{

}

bool EntityPropertyColorFeel::eventFilter(QObject* object, QEvent* event)
{
    if(object == m_UI->m_ColorBox && event->type() == QEvent::MouseButtonPress) 
    {
        onColorBoxClicked();
        return false;
    }
    return false;
}

void EntityPropertyColorFeel::onColorBoxClicked()
{
    QColorDialog* colorDialog(Sandbox::getInstance()->getColorPicker());
    QMetaObject::Connection connection(connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(onColorChanged(const QColor&))));
    
    he::vec4 color(he::ge::PropertyConverterVec4::fromString(he::String(m_UI->m_ColorBox->text().toUtf8())));
    colorDialog->setCurrentColor(QColor::fromRgbF(color.x, color.y, color.z, color.w));

    QString oldValue(m_UI->m_ColorBox->text());
    const int result(colorDialog->exec());
    if (result == QDialog::Accepted)
    {

    }
    else if (m_UI->m_ColorBox->text() != oldValue)
    {
        he::String value(oldValue.toUtf8());
        ValueChanged(value);        
    }
    disconnect(connection);
}

void EntityPropertyColorFeel::onDirtyChanged( const bool newDirty )
{
    if (newDirty)
    {
        m_UI->m_ColorBox->setStyleSheet(
            "border: 2px solid green;");
    }
    else
    {
        m_UI->m_ColorBox->setStyleSheet("");
    }
}

void EntityPropertyColorFeel::UpdateBackground( const he::String& colorStr )
{
    he::vec4 color(he::ge::PropertyConverterVec4::fromString(colorStr));
    UpdateBackground(QColor::fromRgbF(color.x, color.y, color.z, color.w));
}

void EntityPropertyColorFeel::UpdateBackground( const QColor& color )
{
    m_UI->m_ColorBox->setStyleSheet(QString("background-color: ") + color.name());
}

}