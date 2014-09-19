#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <QMainWindow>

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

public slots:
    void createNewGraph();
    void tabCloseRequested(const int tab);
    
private:
    Ui::MaterialEditor* m_UI;
};

}
#endif // MAINWINDOW_H
