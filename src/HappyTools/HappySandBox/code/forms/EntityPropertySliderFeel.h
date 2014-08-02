#ifndef ENTITYPROPERTYSLIDERFEEL_H
#define ENTITYPROPERTYSLIDERFEEL_H

#include "EntityPropertyFeel.h"

#include <QWidget>

namespace Ui {
class EntityPropertySliderFeel;
}

namespace hs {

class EntityPropertySliderFeel : public QWidget, public EntityPropertyFeel
{
    Q_OBJECT

public:
    explicit EntityPropertySliderFeel(QWidget *parent = 0);
    ~EntityPropertySliderFeel();

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

    Ui::EntityPropertySliderFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
