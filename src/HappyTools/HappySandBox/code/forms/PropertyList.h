#ifndef ENTITYPROPERTYLIST_H
#define ENTITYPROPERTYLIST_H

#include <QWidget>
#include <QTableWidget>

#include <FixedStringMap.h>

class QLineEdit;

namespace Ui {
class PropertyList;
}

namespace he {
namespace ge {
    struct PropertyDesc;
    class Property;
}
}

namespace hs {
class PropertyListItem;

class PropertyList : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyList(QWidget *parent = 0);
    ~PropertyList();

    void addProperty(const he::ge::PropertyDesc& prop);
    void setValue(const he::FixedString& prop, const he::String& value);
    void setValueMixed(const he::FixedString& prop);

    void clear();

    he::event1<void, he::ge::Property*> ValueChanged;

private:
    void getRowContents(const he::ge::PropertyDesc& prop, QTableWidgetItem*& outName, hs::PropertyListItem*& outItem, QWidget*& outWidget );

    he::FixedStringMap<PropertyListItem*> m_Items;
    Ui::PropertyList* m_UI;
    int m_CurrentRow;
};
}

#endif // ENTITYPROPERTYLIST_H
