#ifndef HAPPYSANDBOX_H
#define HAPPYSANDBOX_H

#include "HappyQtWidget.h"
#pragma warning(disable:4127)
#include <QtGui/QMainWindow>
#include "ui_HappySandBox.h"
#pragma warning(default:4127)

class HappySandBox : public QMainWindow
{
    Q_OBJECT

public:
    HappySandBox(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~HappySandBox();

    he::gfx::HappyQtWidget* getGameWidget();

private:
    Ui::HappySandBoxClass ui;
};

#endif // HAPPYSANDBOX_H
