#ifndef ENTITYPROPERTYLIST_H
#define ENTITYPROPERTYLIST_H

#include <QWidget>

namespace Ui {
class EntityPropertyList;
}

namespace he {
namespace ge {
    struct PropertyDesc;
}
}

class EntityPropertyList : public QWidget
{
    Q_OBJECT

public:
    explicit EntityPropertyList(QWidget *parent = 0);
    ~EntityPropertyList();

    void AddProperty(const he::ge::PropertyDesc& prop);

private:
    Ui::EntityPropertyList* m_UI;
    int m_CurrentRow;
};

#endif // ENTITYPROPERTYLIST_H
