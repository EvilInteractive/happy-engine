#ifndef ENTITYPROPERTYSLIDERFEEL_H
#define ENTITYPROPERTYSLIDERFEEL_H

#include <QWidget>

namespace Ui {
class EntityPropertySliderFeel;
}

namespace hs {

class EntityPropertySliderFeel : public QWidget
{
    Q_OBJECT

public:
    explicit EntityPropertySliderFeel(QWidget *parent = 0);
    ~EntityPropertySliderFeel();

private:
    Ui::EntityPropertySliderFeel* m_UI;
};

}
#endif // ENTITYPROPERTYFEEL_H
