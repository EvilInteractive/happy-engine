#include "HappyQtTest.h"

HappyQtTest::HappyQtTest(QWidget *parent, Qt::WFlags flags)
    : /*qt::*/QMainWindow(parent, flags)
{
    ui.setupUi(this);
}

HappyQtTest::~HappyQtTest()
{

}

he::gfx::HappyQtWidget* HappyQtTest::getGameWidget()
{
    return ui.centralWidget->findChild<he::gfx::HappyQtWidget*>("gameWidget");
}