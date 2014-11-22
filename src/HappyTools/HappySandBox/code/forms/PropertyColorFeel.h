#ifndef ENTITYPROPERTYCOLORFEEL_H
#define ENTITYPROPERTYCOLORFEEL_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertyColorFeel;
}

namespace hs {

class PropertyColorFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertyColorFeel(QWidget *parent = 0);
    ~PropertyColorFeel();

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

    Ui::PropertyColorFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
