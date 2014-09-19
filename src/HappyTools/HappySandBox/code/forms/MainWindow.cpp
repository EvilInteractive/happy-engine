#include "HappySandBoxPCH.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Sandbox.h"

#include "plugins/materialGenerator/forms/MaterialEditor.h"

namespace hs {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_UI(NEW Ui::MainWindow),
    m_MaterialEditor(NEW MaterialEditor())
{
    m_UI->setupUi(this);

    connect(m_UI->actionMaterialEditor, SIGNAL(triggered()), this, SLOT(openMaterialEditor()));
}

MainWindow::~MainWindow()
{
    delete m_UI;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    delete m_MaterialEditor;
    m_MaterialEditor = nullptr;

    hs::Sandbox::getInstance()->quit();

    QMainWindow::closeEvent(event);
}

GameWidget* MainWindow::getGameWidget() const
{
    return m_UI->gameWidget;
}

void MainWindow::openMaterialEditor()
{
    if (!m_MaterialEditor->isOpen())
    {
        m_MaterialEditor->open();
    }
    else
    {
        m_MaterialEditor->bringToFront();
    }
}

}
