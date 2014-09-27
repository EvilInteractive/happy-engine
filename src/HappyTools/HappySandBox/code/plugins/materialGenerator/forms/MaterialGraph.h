#ifndef MaterialGraph_H
#define MaterialGraph_H

#include "forms/nodegraph/NodeGraph.h"

namespace Ui {
    class MaterialGraph;
}

namespace hs {
class MaterialGraph : public NodeGraph
{
    Q_OBJECT
public:
    explicit MaterialGraph(QWidget* parent);
    ~MaterialGraph();

private:
};

}
#endif // MAINWINDOW_H
