#ifndef PropertyCheckBoxFeel_H
#define PropertyCheckBoxFeel_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertyCheckBoxFeel;
}

namespace hs {

class PropertyCheckBoxFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertyCheckBoxFeel(QWidget *parent = 0);
    ~PropertyCheckBoxFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

private slots:
    void onCheckedChanged(const int newState);

private:
    Ui::PropertyCheckBoxFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
