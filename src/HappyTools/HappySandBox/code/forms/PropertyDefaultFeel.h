#ifndef ENTITYPROPERTYDEFAULTFEEL_H
#define ENTITYPROPERTYDEFAULTFEEL_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertyDefaultFeel;
}

namespace hs {

class PropertyDefaultFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertyDefaultFeel(QWidget *parent = 0);
    ~PropertyDefaultFeel();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

private slots:
    void onEditingFinished();
    void onTextModified(const QString& text);

private:
    Ui::PropertyDefaultFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
