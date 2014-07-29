#include "HappySandBoxPCH.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Sandbox.h"

namespace hs {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_UI(NEW Ui::MainWindow)
{
    m_UI->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_UI;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    hs::Sandbox::getInstance()->quit();
    QMainWindow::closeEvent(event);
}

GameWidget* MainWindow::getGameWidget() const
{
    return m_UI->gameWidget;
}

}
