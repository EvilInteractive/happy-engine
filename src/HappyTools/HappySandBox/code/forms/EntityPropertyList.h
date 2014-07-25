#ifndef ENTITYPROPERTYLIST_H
#define ENTITYPROPERTYLIST_H

#include <QWidget>
#include <QTableWidget>

#include <FixedStringMap.h>

class QLineEdit;

namespace Ui {
class EntityPropertyList;
}

namespace he {
namespace ge {
    struct PropertyDesc;
    class Property;
}
}

namespace hs {
class EntityPropertyListItem;

class EntityPropertyList : public QWidget
{
    Q_OBJECT

public:
    explicit EntityPropertyList(QWidget *parent = 0);
    ~EntityPropertyList();

    void setComponentType(const he::FixedString& type) { m_Component = type; }

    void addProperty(const he::ge::PropertyDesc& prop);
    void setValue(const he::FixedString& prop, const he::String& value);
    void setValueMixed(const he::FixedString& prop);

    void clear();

    he::event2<void, const he::FixedString& /*component*/, he::ge::Property*> ValueChanged;

private:
    void getRowContents(const he::FixedString& component, const he::ge::PropertyDesc& prop, QTableWidgetItem*& outName, hs::EntityPropertyListItem*& outItem, QWidget*& outWidget );

    he::FixedString m_Component;
    he::FixedStringMap<EntityPropertyListItem*> m_Items;
    Ui::EntityPropertyList* m_UI;
    int m_CurrentRow;
};
}

#endif // ENTITYPROPERTYLIST_H
