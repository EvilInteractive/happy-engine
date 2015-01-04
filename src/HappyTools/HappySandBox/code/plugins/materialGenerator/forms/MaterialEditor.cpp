#include "HappySandBoxPCH.h"
#include "MaterialEditor.h"
#include "ui_MaterialEditor.h"

#include "MaterialGraph.h"
#include "../system/MaterialGeneratorNodeTypes.h"
#include "system/Options.h"

#include <QMessageBox>
#include <QTreeWidgetItem>

namespace hs {

MaterialEditor::MaterialEditor() :
    m_UI(NEW Ui::MaterialEditor())
{
    m_UI->setupUi(this);

    connect(m_UI->actionNew, SIGNAL(triggered()), this, SLOT(createNewGraph()));
    connect(m_UI->actionCompile, SIGNAL(triggered()), this, SLOT(compile()));
    connect(m_UI->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(m_UI->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));

    he::eventCallback1<void, he::ge::Property*> valueChangeCallback(std::bind(&MaterialEditor::onValueChange, this, std::placeholders::_1));
    m_UI->propertyList->ValueChanged += valueChangeCallback;
   
    he::FixedString rootItems[] = { HSFS::strRootNormalDraw };
    he::FixedString constItems[] = { HSFS::strFloat1, HSFS::strFloat2, HSFS::strFloat3, HSFS::strFloat4 };
    he::FixedString mathItems[] =  { HSFS::strAbs, HSFS::strCos, HSFS::strSin, HSFS::strCeil, HSFS::strFloor, HSFS::strFrac, HSFS::strOneMin, HSFS::strNormalize,
        HSFS::strSign, HSFS::strAdd, HSFS::strDivide, HSFS::strMin, HSFS::strMax, HSFS::strMultiply, HSFS::strSubtract, HSFS::strCross,
        HSFS::strDistance, HSFS::strDistanceSqr, HSFS::strDot, HSFS::strPower, HSFS::strReflect, HSFS::strClamp, HSFS::strLerp };
    he::FixedString vectorItems[] = { HSFS::strComposeVector, HSFS::strSwizzle };

    const size_t listCount(4);
    const char* names[listCount] = { "Root", "Const", "Math", "Vector" };
    int listItemCount[listCount] = { sizeof(rootItems) / sizeof(he::FixedString), sizeof(constItems) / sizeof(he::FixedString), sizeof(mathItems) / sizeof(he::FixedString), sizeof(vectorItems) / sizeof(he::FixedString) };
    he::FixedString* listItems[listCount] = { rootItems, constItems, mathItems, vectorItems };
    for (size_t i(0); i < listCount; ++i)
    {
        QTreeWidgetItem* rootNodes(NEW QTreeWidgetItem());
        rootNodes->setData(0, Qt::DisplayRole, names[i]);
        rootNodes->setData(0, Qt::UserRole, he::HEFS::str.c_str());

        he::FixedString* items(listItems[i]);
        const size_t itemCount(listItemCount[i]);
        for (size_t j(0); j < itemCount; ++j)
        {
            QTreeWidgetItem* sub(NEW QTreeWidgetItem());
            sub->setData(0, Qt::DisplayRole, items[j].c_str());
            sub->setData(0, Qt::UserRole, items[j].c_str());
            rootNodes->addChild(sub);
        }
        m_UI->nodeTypeList->addTopLevelItem(rootNodes);
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

he::FixedString MaterialEditor::getActiveCreateNode() const
{
    he::FixedString result(he::HEFS::str);

    QTreeWidgetItem* item(m_UI->nodeTypeList->currentItem());
    if (item)
    {
        result = he::FixedString::fromString(item->data(0, Qt::UserRole).toString().toUtf8().data());
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

void MaterialEditor::save()
{
    MaterialGraph* graph(he::checked_cast<MaterialGraph*>(m_UI->tabWidget->currentWidget()));
    if (graph)
    {
        graph->save(Options::getInstance()->getPathOption(HSFS::strEditorDataPath).append("Shaders/testShader1.hs"));
    }
}

void MaterialEditor::clearProperties()
{
    m_UI->propertyList->clear();
}

void MaterialEditor::addProperty( const he::ge::PropertyDesc& propDesc )
{
    m_UI->propertyList->addProperty(propDesc);
}

void MaterialEditor::onValueChange( he::ge::Property* prop )
{
    MaterialGraph* graph(he::checked_cast<MaterialGraph*>(m_UI->tabWidget->currentWidget()));
    if (graph)
    {
        graph->onPropertyValueChanged(prop);
    }
    emit propertyChanged(prop);
}

}
