#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <QMainWindow>
#include "../system/MaterialGeneratorNodeTypes.h"

namespace Ui {
    class MaterialEditor;
}

namespace hs {
class MaterialGraph;
class MaterialEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit MaterialEditor();
    ~MaterialEditor();

    bool isOpen();
    void open();

    void bringToFront();

    MaterialGeneratorNodeType getActiveCreateNode() const;

public slots:
    void createNewGraph();
    void tabCloseRequested(const int tab);
    void compile();
    
private:
    Ui::MaterialEditor* m_UI;
};

}
#endif // MAINWINDOW_H
