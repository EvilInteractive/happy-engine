#include "HappySandBoxPCH.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Sandbox.h"

namespace hs {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    hs::Sandbox::getInstance()->quit();
    QMainWindow::closeEvent(event);
}

GameWidget* MainWindow::getGameWidget() const
{
    return ui->gameWidget;
}

}
