//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _HS_NODEGRAPH_H_
#define _HS_NODEGRAPH_H_
#pragma once

#include "forms/RenderWidget.h"

#include <IDrawable2D.h>
#include <ITickable.h>
#include <Text.h>

namespace hs {
class NodeGraphNode;
struct NodeGraphDrawContext;
class NodeGraphNodeConnector;
class NodeGraph :  public RenderWidget, public he::gfx::IDrawable2D, public he::ge::ITickable
{
    Q_OBJECT
public:
    explicit NodeGraph(QWidget *parent = 0);
    virtual ~NodeGraph();

    void activate();
    void deactivate();

    virtual void destroy() override;

    void addNode(NodeGraphNode* node, const he::vec2& screenPos);

    virtual void tick(float dTime);
    virtual void draw2D(he::gui::Canvas2D* canvas) override;

    bool isEdited() const { return false; }

protected:
    // Called when left mouse is pressed (only return a node if you want to create one on click (with a shortcut))
    virtual NodeGraphNode* createNode() = 0;
    virtual void destroyNode(NodeGraphNode* node) = 0;

    // Called when a connection is made between two connectors, returns if successful
    virtual bool connect(NodeGraphNodeConnector* const from, NodeGraphNodeConnector* const to);
    virtual bool disconnect(NodeGraphNodeConnector* const connection);

    // If the selection changed
    const he::PrimitiveList<NodeGraphNode*>& getSelection() const { return m_SelectedNodes; }
    virtual void onSelectionChanged() {}

private:
    enum State
    {
        State_Idle,
        State_StartPan,
        State_Pan,
        State_StartMoveNode,
        State_MoveNode,
        State_ConnectNode
    };

    // Updates
    void updateStates(const float dTime);
    void updateIdleState(const float dTime);
    void updateStartPanState(const float dTime);
    void updatePanState(const float dTime);
    void updateStartMoveNodeState(const float dTime);
    void updateMoveNodeState(const float dTime);
    void updateConnectNodeState(const float dTime);

    void updateZoom(const float dTime);

    bool doNodeHover(const he::vec2& worldPos);
    bool doNodeSelect(const bool keepSelection, const bool removeSelection, const bool removeSelectionIfSelected);
    void doNodeMove(const he::vec2& worldDelta);
    bool doConnectStart();
    bool doConnectEnd();

    // Draw
    void drawBackground(const NodeGraphDrawContext& context);
    void drawNodes(const NodeGraphDrawContext& context);
    void drawDebug(const NodeGraphDrawContext& context);

    // Helpers
    he::vec2 screenToWorldPos(const he::vec2& screenPos) const;
    he::vec2 worldToScreenPos(const he::vec2& worldPos) const;

    // Members
    he::gfx::View* m_View;
    he::gfx::Renderer2D* m_2DRenderer;

    bool m_Active;

    State m_State;
    he::vec2 m_GrabWorldPos;
    he::vec2 m_Offset;
    float m_Scale;

    he::PrimitiveList<NodeGraphNode*> m_Nodes;
    he::PrimitiveList<NodeGraphNode*> m_SelectedNodes;
    NodeGraphNode* m_HoverNode;
    NodeGraphNodeConnector* m_HoverConnector;
    NodeGraphNodeConnector* m_ConnectingConnector;

    he::gui::Text m_DebugText;

    //Disable default copy constructor and default assignment operator
    NodeGraph(const NodeGraph&);
    NodeGraph& operator=(const NodeGraph&);
};

} //end namespace

#endif
