#include "HappySandBoxPCH.h"
#include "EntityPropertySliderFeel.h"
#include "ui_EntityPropertySliderFeel.h"

#include <PropertyConverter.h>

namespace hs {

EntityPropertySliderFeel::EntityPropertySliderFeel(QWidget *parent) :
    QWidget(parent),
    m_Decimals(0),
    m_UI(NEW Ui::EntityPropertySliderFeel)
{
    m_UI->setupUi(this);

    connect(m_UI->m_Slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderMoved(int)));
    connect(m_UI->m_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onNumericChanged(double)));

    m_UI->m_SpinBox->setDecimals(m_Decimals);
}

EntityPropertySliderFeel::~EntityPropertySliderFeel()
{
    delete m_UI;
}

void EntityPropertySliderFeel::setValue(const he::String& value)
{
    const float val(he::ge::PropertyConverterFloat::fromString(value));
    m_UI->m_Slider->setValue(static_cast<int>(val * (m_Decimals + 1.0f)));
    m_UI->m_SpinBox->setValue(val);
}

void EntityPropertySliderFeel::setValueMixed()
{
    m_UI->m_SpinBox->clear();

}

void EntityPropertySliderFeel::onDirtyChanged(const bool newDirty)
{

}

void EntityPropertySliderFeel::setDecimals(const int numb)
{
    if (m_Decimals != numb)
    {
        const float rangeMin(m_UI->m_Slider->minimum() / (m_Decimals + 1.0f));
        const float rangeMax(m_UI->m_Slider->maximum() / (m_Decimals + 1.0f));
        const float tickInterval(m_UI->m_Slider->tickInterval() / (m_Decimals + 1.0f));
        const float val(m_UI->m_Slider->value() / (m_Decimals + 1.0f));
        m_Decimals = numb;
        setRange(rangeMin, rangeMax);
        setTicksInterval(tickInterval);
        m_UI->m_Slider->setValue(static_cast<int>(val * (m_Decimals + 1.0f)));
        m_UI->m_SpinBox->setDecimals(m_Decimals);
    }
}

void EntityPropertySliderFeel::setRange(const float min, const float max)
{
    m_UI->m_Slider->setRange(static_cast<int>(min * (m_Decimals + 1)), static_cast<int>(max * (m_Decimals + 1)));
    m_UI->m_SpinBox->setRange(min, max);
}

void EntityPropertySliderFeel::setTicksInterval(const float tickinterval)
{
    m_UI->m_Slider->setTickInterval(static_cast<int>(tickinterval * (m_Decimals + 1)));
}

void EntityPropertySliderFeel::onSliderMoved(const int value)
{
    const float newValue(value / (m_Decimals + 1.0f));
    he::String str(he::ge::PropertyConverterFloat::toString(newValue, m_Decimals));
    ValueChanged(str);
}

void EntityPropertySliderFeel::onNumericChanged(const double value)
{
    he::String str(he::ge::PropertyConverterFloat::toString(static_cast<float>(value), m_Decimals));
    ValueChanged(str);
}

}
