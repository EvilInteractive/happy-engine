#ifndef HAPPYQTTEST_H
#define HAPPYQTTEST_H

#include "HappyQtWidget.h"
#pragma warning(disable:4127)
#include <QtGui/QMainWindow>
#include "ui_HappyQtTest.h"
#pragma warning(default:4127)

class HappyQtTest : public /*qt::*/QMainWindow
{
    Q_OBJECT

public:
    HappyQtTest(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~HappyQtTest();
    
    he::gfx::HappyQtWidget* getGameWidget();
private:
    /*qt::*/Ui::HappyQtTestClass ui;
};

#endif // HAPPYQTTEST_H
