#include "HappySandBoxPCH.h"
#include "MaterialEditor.h"
#include "ui_MaterialEditor.h"

#include "MaterialGraph.h"
#include "../system/MaterialGeneratorNodeTypes.h"

#include <QMessageBox>
#include <QTreeWidgetItem>

namespace hs {

MaterialEditor::MaterialEditor() :
    m_UI(NEW Ui::MaterialEditor())
{
    m_UI->setupUi(this);

    connect(m_UI->actionNew, SIGNAL(triggered()), this, SLOT(createNewGraph()));
    connect(m_UI->actionCompile, SIGNAL(triggered()), this, SLOT(compile()));

    connect(m_UI->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));

    // Root
    {
        QTreeWidgetItem* rootNodes(NEW QTreeWidgetItem());
        rootNodes->setData(0, Qt::DisplayRole, "Root");
        rootNodes->setData(0, Qt::UserRole, MaterialGeneratorNodeType_Unassigned);

        for (size_t i(MaterialGeneratorNodeType_ROOT); i < MaterialGeneratorNodeType_ROOT_MAX; ++i)
        {
            QTreeWidgetItem* sub(NEW QTreeWidgetItem());
            sub->setData(0, Qt::DisplayRole, materialGeneratorNodeTypeToString(he::checked_numcast<MaterialGeneratorNodeType>(i)));
            sub->setData(0, Qt::UserRole, i);
            rootNodes->addChild(sub);
        }
        m_UI->nodeTypeList->addTopLevelItem(rootNodes);
    }

    // Const
    {
        QTreeWidgetItem* constNodes(NEW QTreeWidgetItem());
        constNodes->setData(0, Qt::DisplayRole, "Const");
        constNodes->setData(0, Qt::UserRole, MaterialGeneratorNodeType_Unassigned);

        for (size_t i(MaterialGeneratorNodeType_CONST); i < MaterialGeneratorNodeType_CONST_MAX; ++i)
        {
            QTreeWidgetItem* sub(NEW QTreeWidgetItem());
            sub->setData(0, Qt::DisplayRole, materialGeneratorNodeTypeToString(he::checked_numcast<MaterialGeneratorNodeType>(i)));
            sub->setData(0, Qt::UserRole, i);
            constNodes->addChild(sub);
        }
        m_UI->nodeTypeList->addTopLevelItem(constNodes);
    }
    
    // Math
    {
        QTreeWidgetItem* math(NEW QTreeWidgetItem());
        math->setData(0, Qt::DisplayRole, "Math");
        math->setData(0, Qt::UserRole, MaterialGeneratorNodeType_Unassigned);

        for (size_t i(MaterialGeneratorNodeType_MATH); i < MaterialGeneratorNodeType_MATH_MAX; ++i)
        {
            QTreeWidgetItem* sub(NEW QTreeWidgetItem());
            sub->setData(0, Qt::DisplayRole, materialGeneratorNodeTypeToString(he::checked_numcast<MaterialGeneratorNodeType>(i)));
            sub->setData(0, Qt::UserRole, i);
            math->addChild(sub);
        }
        m_UI->nodeTypeList->addTopLevelItem(math);
    }

    // Vector
    {
        QTreeWidgetItem* vec(NEW QTreeWidgetItem());
        vec->setData(0, Qt::DisplayRole, "Vector");
        vec->setData(0, Qt::UserRole, MaterialGeneratorNodeType_Unassigned);

        for (size_t i(MaterialGeneratorNodeType_VECTOR); i < MaterialGeneratorNodeType_VECTOR_MAX; ++i)
        {
            QTreeWidgetItem* sub(NEW QTreeWidgetItem());
            sub->setData(0, Qt::DisplayRole, materialGeneratorNodeTypeToString(he::checked_numcast<MaterialGeneratorNodeType>(i)));
            sub->setData(0, Qt::UserRole, i);
            vec->addChild(sub);
        }
        m_UI->nodeTypeList->addTopLevelItem(vec);
    }
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
    newTab->init(this);
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

hs::MaterialGeneratorNodeType MaterialEditor::getActiveCreateNode() const
{
    MaterialGeneratorNodeType result(MaterialGeneratorNodeType_Unassigned);

    QTreeWidgetItem* item(m_UI->nodeTypeList->currentItem());
    if (item)
    {
        result = he::checked_numcast<MaterialGeneratorNodeType>(item->data(0, Qt::UserRole).toInt());
    }
    return result;
}

void MaterialEditor::compile()
{
    MaterialGraph* graph(he::checked_cast<MaterialGraph*>(m_UI->tabWidget->currentWidget()));
    if (graph)
    {
        graph->compile();
    }
}

}
