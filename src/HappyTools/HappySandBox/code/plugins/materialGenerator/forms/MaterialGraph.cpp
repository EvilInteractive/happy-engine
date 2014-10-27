#include "HappySandBoxPCH.h"
#include "MaterialGraph.h"

#include "forms/nodegraph/NodeGraphNode.h"
#include "forms/nodegraph/NodeGraphNodeAttachments.h"

#include <ControlsManager.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <ContentManager.h>
#include <Texture2D.h>

namespace hs {

MaterialGraph::MaterialGraph(QWidget* parent) :
    NodeGraph(parent)
{
}

MaterialGraph::~MaterialGraph()
{
}

NodeGraphNode* MaterialGraph::createNode()
{
    const he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IKeyboard* const keyboard(controls->getKeyboard(getHandle()));
    NodeGraphNode* node(nullptr);
    if (keyboard->isKeyDown(he::io::Key_Shift))
    {
        const size_t inputs(rand() % 5 + 1);
        const size_t outputs(rand() % 5 + 1);
        const bool addTexture((rand() % 5) == 0);

        char* names[] = { "position", "x", "y", "z", "color", "input1", "input2", "diffuse" };
        const int nameCount(sizeof(names) / sizeof(char*));

        node = NEW NodeGraphNode();
        node->startEdit();
        for (size_t i(0); i < inputs; ++i)
        {
            NodeGraphNodeTextConnector* connector(NEW NodeGraphNodeTextConnector());
            connector->init(eNodeGraphNodeConnectorType_Input, names[rand()%nameCount]);
            node->addAttachment(connector);
        }
        if (addTexture)
        {
            NodeGraphNodeTextureAttachment* connector(NEW NodeGraphNodeTextureAttachment());
            const he::gfx::Texture2D* tex(CONTENT->asyncLoadTexture2D("engine/he_logo.png"));
            connector->init(tex, he::vec2(64, 64));
            tex->release();
            node->addAttachment(connector);
        }
        for (size_t i(0); i < outputs; ++i)
        {
            NodeGraphNodeTextConnector* connector(NEW NodeGraphNodeTextConnector());
            connector->init(eNodeGraphNodeConnectorType_Output, names[rand()%nameCount]);
            node->addAttachment(connector);
        }
        node->endEdit();
    }
    return node;
}

bool MaterialGraph::connect( NodeGraphNodeConnector* const from, NodeGraphNodeConnector* const to )
{
    return NodeGraph::connect(from, to);
}

}
