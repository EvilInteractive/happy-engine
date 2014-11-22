#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <QMainWindow>
#include "../system/MaterialGeneratorNodeTypes.h"

#include <ShaderGeneratorEnums.h>

namespace Ui {
    class MaterialEditor;
}

namespace he {
namespace ge {
    struct PropertyDesc;
    class Property;
}}

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

    void clearProperties();
    void addProperty(const he::ge::PropertyDesc& propDesc);

    MaterialGeneratorNodeType getActiveCreateNode() const;

public slots:
    void createNewGraph();
    void tabCloseRequested(const int tab);
    void compile();

signals:
    void propertyChanged(he::ge::Property* val);
    
private:
    void onValueChange(he::ge::Property* prop);
    Ui::MaterialEditor* m_UI;
};

}
#endif // MAINWINDOW_H
