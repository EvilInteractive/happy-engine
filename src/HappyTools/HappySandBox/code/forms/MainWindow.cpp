#include "HappySandBoxPCH.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Sandbox.h"
#include "OptionsForm.h"

#include "plugins/materialGenerator/forms/MaterialEditor.h"

namespace hs {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_UI(NEW Ui::MainWindow),
    m_MaterialEditor(NEW MaterialEditor()),
    m_OptionsForm(NEW OptionsForm(NULL))
{
    m_OptionsForm->activateWindow();
    m_UI->setupUi(this);

    connect(m_UI->actionMaterialEditor, SIGNAL(triggered()), this, SLOT(openMaterialEditor()));
    connect(m_UI->actionOptions, SIGNAL(triggered()), this, SLOT(openOptionsForm()));
}

MainWindow::~MainWindow()
{
    delete m_UI;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    delete m_MaterialEditor;
    m_MaterialEditor = nullptr;

    delete m_OptionsForm;
    m_OptionsForm = nullptr;

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

void MainWindow::openOptionsForm()
{
    if (m_OptionsForm->isHidden())
    {
        m_OptionsForm->load();
        m_OptionsForm->show();
        m_OptionsForm->activateWindow();
    }
    m_OptionsForm->raise();
}

}
