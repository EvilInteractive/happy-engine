#ifndef ENTITYPROPERTYDROPDOWNFEEL_H
#define ENTITYPROPERTYDROPDOWNFEEL_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertyDropDownFeel;
}

namespace hs {

class PropertyDropDownFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertyDropDownFeel(QWidget *parent = 0);
    ~PropertyDropDownFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

    void addValue(const he::String& value);

private slots:
    void onSelectionChanged(const QString& text);

private:
    Ui::PropertyDropDownFeel* m_UI;
    bool m_HoldEvents;
};

}
#endif // ENTITYPROPERTYFEEL_H
