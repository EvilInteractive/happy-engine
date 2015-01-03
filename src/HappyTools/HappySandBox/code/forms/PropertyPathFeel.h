#ifndef ENTITYPropertyPathFeel_H
#define ENTITYPropertyPathFeel_H

#include "PropertyFeel.h"

#include <QWidget>

namespace Ui {
class PropertyPathFeel;
}

namespace hs {

class PropertyPathFeel : public QWidget, public PropertyFeel
{
    Q_OBJECT

public:
    explicit PropertyPathFeel(QWidget *parent = 0);
    virtual ~PropertyPathFeel();

    void setOpenFile(const he::String& filter = he::String());
    void setOpenDirectory();

    void setValue(const he::String& value) override;
    void setValueMixed() override;
    void onDirtyChanged(const bool newDirty) override;

private slots:
    void onEditingFinished();
    void onTextModified(const QString& text);
    void onOpenPathDialog();

protected:
    Ui::PropertyPathFeel* m_UI;

    he::String m_Filter;
    bool m_IsDirectory;
};

}
#endif // ENTITYPROPERTYFEEL_H
