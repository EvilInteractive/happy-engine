#include "HappySandBoxPCH.h"
#include "PropertyColorFeel.h"
#include "ui_PropertyColorFeel.h"

#include "Sandbox.h"

#include <PropertyConverter.h>

#include <QColorDialog>

namespace hs {

PropertyColorFeel::PropertyColorFeel(QWidget *parent) :
    QWidget(parent),
    m_UI(NEW Ui::PropertyColorFeel)
{
    m_UI->setupUi(this);
    m_UI->m_ColorBox->installEventFilter(this);
}

PropertyColorFeel::~PropertyColorFeel()
{
    delete m_UI;
}

void PropertyColorFeel::setValue( const he::String& value )
{
    m_UI->m_ColorBox->setText(value.c_str());
    UpdateBackground(value);
    setDirty(false);
}

void PropertyColorFeel::setValueMixed()
{
    m_UI->m_ColorBox->setPlaceholderText("<Mixed Values>");
    m_UI->m_ColorBox->setText("");
    setDirty(false);
}

void PropertyColorFeel::onColorChanged(const QColor& color)
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

void PropertyColorFeel::onColorApplied()
{

}

bool PropertyColorFeel::eventFilter(QObject* object, QEvent* event)
{
    if(object == m_UI->m_ColorBox && event->type() == QEvent::MouseButtonPress) 
    {
        onColorBoxClicked();
        return false;
    }
    return false;
}

void PropertyColorFeel::onColorBoxClicked()
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

void PropertyColorFeel::onDirtyChanged( const bool newDirty )
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

void PropertyColorFeel::UpdateBackground( const he::String& colorStr )
{
    he::vec4 color(he::ge::PropertyConverterVec4::fromString(colorStr));
    UpdateBackground(QColor::fromRgbF(color.x, color.y, color.z, color.w));
}

void PropertyColorFeel::UpdateBackground( const QColor& color )
{
    m_UI->m_ColorBox->setStyleSheet(QString("background-color: ") + color.name());
}

}