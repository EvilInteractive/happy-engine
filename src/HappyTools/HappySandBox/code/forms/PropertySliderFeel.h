#ifndef ENTITYPROPERTYSLIDERFEEL_H
#define ENTITYPROPERTYSLIDERFEEL_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertySliderFeel;
}

namespace hs {

class PropertySliderFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertySliderFeel(QWidget *parent = 0);
    ~PropertySliderFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

    void setDecimals(const int numb);
    void setRange(const float min, const float max);
    void setTicksInterval(const float tickinterval);

private slots:
    void onSliderMoved(const int value);
    void onNumericChanged(const double value);

private:    
    int m_Decimals;

    Ui::PropertySliderFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
