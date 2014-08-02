#ifndef ENTITYPROPERTYDROPDOWNFEEL_H
#define ENTITYPROPERTYDROPDOWNFEEL_H

#include "EntityPropertyFeel.h"

#include <QWidget>

namespace Ui {
class EntityPropertyDropDownFeel;
}

namespace hs {

class EntityPropertyDropDownFeel : public QWidget, public EntityPropertyFeel
{
    Q_OBJECT

public:
    explicit EntityPropertyDropDownFeel(QWidget *parent = 0);
    ~EntityPropertyDropDownFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

    void addValue(const he::String& value);

private slots:
    void onSelectionChanged(const QString& text);

private:
    Ui::EntityPropertyDropDownFeel* m_UI;
    bool m_HoldEvents;
};

}
#endif // ENTITYPROPERTYFEEL_H
