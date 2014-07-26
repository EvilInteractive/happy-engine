#ifndef ENTITYPROPERTYDEFAULTFEEL_H
#define ENTITYPROPERTYDEFAULTFEEL_H

#include "EntityPropertyFeel.h"

#include <QWidget>

namespace Ui {
class EntityPropertyDefaultFeel;
}

namespace hs {

class EntityPropertyDefaultFeel : public QWidget, public EntityPropertyFeel
{
    Q_OBJECT

public:
    explicit EntityPropertyDefaultFeel(QWidget *parent = 0);
    ~EntityPropertyDefaultFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

private slots:
    void onEditingFinished();
    void onTextModified(const QString& text);

private:
    Ui::EntityPropertyDefaultFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
