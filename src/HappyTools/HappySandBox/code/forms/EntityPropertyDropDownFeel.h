#ifndef ENTITYPROPERTYDROPDOWNFEEL_H
#define ENTITYPROPERTYDROPDOWNFEEL_H

#include <QWidget>

namespace Ui {
class EntityPropertyDropDownFeel;
}

namespace hs {

class EntityPropertyDropDownFeel : public QWidget
{
    Q_OBJECT

public:
    explicit EntityPropertyDropDownFeel(QWidget *parent = 0);
    ~EntityPropertyDropDownFeel();

private:
    Ui::EntityPropertyDropDownFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
