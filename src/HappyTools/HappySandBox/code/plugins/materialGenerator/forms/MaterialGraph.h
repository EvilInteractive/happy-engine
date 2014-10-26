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

protected:
    // Called when left mouse is pressed (only return a node if you want to create one on click (with a shortcut))
    virtual NodeGraphNode* createNode() override;

    // Called when a connection is made between two connectors, returns if successful
    virtual bool connect(NodeGraphNodeConnector* const from, NodeGraphNodeConnector* const to) override;

private:
};

}
#endif // MAINWINDOW_H
