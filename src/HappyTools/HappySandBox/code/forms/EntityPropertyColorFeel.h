#ifndef ENTITYPROPERTYCOLORFEEL_H
#define ENTITYPROPERTYCOLORFEEL_H

#include "EntityPropertyFeel.h"

#include <QWidget>

namespace Ui {
class EntityPropertyColorFeel;
}

namespace hs {

class EntityPropertyColorFeel : public QWidget, public EntityPropertyFeel
{
    Q_OBJECT

public:
    explicit EntityPropertyColorFeel(QWidget *parent = 0);
    ~EntityPropertyColorFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;
    bool eventFilter(QObject* object, QEvent* event);

private slots:
    void onColorChanged(const QColor& color);
    void onColorApplied();
    void onColorBoxClicked();

private:
    void UpdateBackground(const he::String& color);
    void UpdateBackground(const QColor& color);

    Ui::EntityPropertyColorFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
