#include "HappySandBox.h"

HappySandBox::HappySandBox(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
}

HappySandBox::~HappySandBox()
{

}

he::gfx::HappyQtWidget* HappySandBox::getGameWidget()
{
    return ui.centralWidget->findChild<he::gfx::HappyQtWidget*>("gameWidget");
}