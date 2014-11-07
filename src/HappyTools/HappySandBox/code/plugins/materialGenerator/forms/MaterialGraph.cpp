#include "HappySandBoxPCH.h"
#include "MaterialGraph.h"

#include "forms/nodegraph/NodeGraphNode.h"
#include "forms/nodegraph/NodeGraphNodeAttachments.h"

#include "MaterialEditor.h"
#include "../system/MaterialGeneratorNodeFactory.h"

#include <ControlsManager.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <ContentManager.h>
#include <Texture2D.h>
#include <ShaderGenerator.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>


namespace hs {

MaterialGraph::MaterialGraph(QWidget* parent) :
    NodeGraph(parent),
    m_Generator(NEW he::ct::ShaderGenerator())
{
    setAcceptDrops(true);
}

MaterialGraph::~MaterialGraph()
{
    destroy();
    delete m_Generator;
}

void MaterialGraph::init( MaterialEditor* parent )
{
    m_Parent = parent;
}

NodeGraphNode* MaterialGraph::createNode()
{
    return nullptr;
}

void MaterialGraph::destroyNode( NodeGraphNode* node )
{
    if (node == m_ActiveRoot)
        m_ActiveRoot = nullptr;
    MaterialGeneratorNodeFactory::getInstance()->destroy(he::checked_cast<MaterialGeneratorNode*>(node));
}

bool MaterialGraph::connect( NodeGraphNodeConnector* const from, NodeGraphNodeConnector* const to )
{
    return NodeGraph::connect(from, to);
}

void MaterialGraph::dragEnterEvent( QDragEnterEvent* event )
{
    QObject* obj(event->source());
    if (obj)
    {
        MaterialGeneratorNodeType type(m_Parent->getActiveCreateNode());
        if (type != MaterialGeneratorNodeType_Unassigned)
        {
            event->acceptProposedAction();
        }
    }
}

void MaterialGraph::dropEvent( QDropEvent* event )
{
    QObject* obj(event->source());
    if (obj)
    {
        MaterialGeneratorNodeType type(m_Parent->getActiveCreateNode());
        if (type != MaterialGeneratorNodeType_Unassigned)
        {
            MaterialGeneratorNode* node(MaterialGeneratorNodeFactory::getInstance()->create(this, type));
            addNode(node, he::vec2(event->posF().x(), event->posF().y()));
            event->acceptProposedAction();

            if (type == MaterialGeneratorNodeType_RootNormalDraw)
                m_ActiveRoot = node;
        }
    }
}

void MaterialGraph::compile()
{
    m_Generator->reset();
    if (m_ActiveRoot)
    {
        m_Generator->compile(CONTENT->getShaderFolderPath().append("Generated"), "TestShader");
    }
}

}
