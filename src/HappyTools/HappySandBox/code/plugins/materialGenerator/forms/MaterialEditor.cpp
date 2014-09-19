#include "HappySandBoxPCH.h"
#include "MaterialEditor.h"
#include "ui_MaterialEditor.h"

#include "MaterialGraph.h"

#include <QMessageBox>

namespace hs {

MaterialEditor::MaterialEditor() :
    m_UI(NEW Ui::MaterialEditor())
{
    m_UI->setupUi(this);

    connect(m_UI->actionNew, SIGNAL(triggered()), this, SLOT(createNewGraph()));
    connect(m_UI->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
}

MaterialEditor::~MaterialEditor()
{
    int index(0);
    MaterialGraph* graph(nullptr);
    while (graph = he::checked_cast<MaterialGraph*>(m_UI->tabWidget->widget(index++)))
    {
        delete graph;
    }
    delete m_UI;
}

bool MaterialEditor::isOpen()
{
    return QMainWindow::isVisible();
}

void MaterialEditor::open()
{
    QMainWindow::show();
}

void MaterialEditor::bringToFront()
{
    if (isOpen())
        QMainWindow::focusWidget();
}

void MaterialEditor::createNewGraph()
{
    MaterialGraph* newTab(NEW MaterialGraph(this));
    m_UI->tabWidget->addTab(newTab, "*New*");
}

void MaterialEditor::tabCloseRequested( const int tab )
{
    MaterialGraph* graph(he::checked_cast<MaterialGraph*>(m_UI->tabWidget->widget(tab)));
    int closeResult(1);
    if (graph->isEdited())
    {
        QMessageBox msgb("Save before close?", "Do you want to save your changes?", QMessageBox::Icon::Question, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        closeResult = msgb.exec();
    }
    if (closeResult == 0)
    {
        // closeResult = SaveDialog::show();
    }
    if (closeResult != 2)
    {
        m_UI->tabWidget->removeTab(tab);
        delete graph;
    }
}

}
