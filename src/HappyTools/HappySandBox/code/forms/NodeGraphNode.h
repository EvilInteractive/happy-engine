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

#ifndef _HS_NODEGRAPHNODE_H_
#define _HS_NODEGRAPHNODE_H_
#pragma once

namespace hs {
class NodeGraphNodeOutput;
class NodeGraphNodeInput;
class NodeGraphNodeDecoration;
class INodeGraphNodeAttachment;
struct NodeGraphDrawContext;

class NodeGraphNode
{
public:
    enum EState
    {
        eState_Normal,
        eState_Hover,
        eState_Selected,

        eState_MAX
    };
    struct Style
    {
        Style();
        he::Color m_Border[eState_MAX];
        he::Color m_Background[eState_MAX];
        he::Color m_Title[eState_MAX];
        float m_TitleSize;
        he::vec4 m_BackgroundMargin;
    };
    NodeGraphNode();
    ~NodeGraphNode();

    // Style
    void setStyle(const Style& style);

    // Attachments
    void addOutput(NodeGraphNodeOutput* output);
    void addInput(NodeGraphNodeInput* input);
    void addDecoration(NodeGraphNodeDecoration* deco);

    void setLayoutDirty() { m_LayoutDirty = true; }

    // State
    bool isInside(const he::vec2& worldPos);

    void setState(const EState state) { m_State = state; }
    EState getState() const { return m_State; }

    void setPosition(const he::vec2& position);
    he::vec2 getPosition() const;

    // Draw
    void draw(const NodeGraphDrawContext& context);

private:
    // Attachments
    void addAttachment(INodeGraphNodeAttachment* att);

    // State
    void updateLayout();

    // Draw
    void drawNodeBackground(const NodeGraphDrawContext& context);
    void drawAttachment(const NodeGraphDrawContext& context, INodeGraphNodeAttachment* attachment);

    // Members
    EState m_State;
    bool m_LayoutDirty;

    he::RectF m_Bound;
    Style m_Style;

    he::PrimitiveList<INodeGraphNodeAttachment*> m_Attachments;
    he::PrimitiveList<NodeGraphNodeOutput*> m_Outputs;
    he::PrimitiveList<NodeGraphNodeInput*> m_Inputs;

    NodeGraphNode(const NodeGraphNode&);
    NodeGraphNode& operator=(const NodeGraphNode&);
};

}

#endif
