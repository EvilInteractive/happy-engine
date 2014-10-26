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

#include <VerticalLayout.h>

namespace he {
namespace gfx
{
    class Texture2D;
}
}


namespace hs {
class NodeGraphNodeAttachment;
struct NodeGraphDrawContext;
class NodeGraphNodeConnector;

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
        const he::gfx::Texture2D* m_NinePatchTextureBackground[eState_MAX];
        he::Color m_Title[eState_MAX];
        float m_TitleSize;

        static Style s_DefaultStyle;

        static void sdmInit();
        static void sdmDestroy();
    };
    NodeGraphNode();
    ~NodeGraphNode();

    // Style
    void setStyle(const Style* style); // Does not take ownership

    // Attachments
    void startEdit();
    void endEdit();
    void addAttachment(NodeGraphNodeAttachment* att);
    
    // State
    bool isInside(const he::vec2& worldPos);
    NodeGraphNodeConnector* pickConnector(const he::vec2& worldPos);

    void setState(const EState state) { m_State = state; }
    EState getState() const { return m_State; }

    void move(const he::vec2& worldDelta);

    void setTitle(const char* title);

    // Draw
    void draw(const NodeGraphDrawContext& context);

private:
    // Draw
    void drawNodeBackground(const NodeGraphDrawContext& context);
    void drawAttachment(const NodeGraphDrawContext& context, NodeGraphNodeAttachment* attachment);

    // Members
    EState m_State;

    const Style* m_Style;

    he::PrimitiveList<NodeGraphNodeAttachment*> m_Attachments;
    
    he::gui::VerticalLayout m_Layout;
    he::gui::Text m_Title;
    he::vec2 m_Position;

    NodeGraphNode(const NodeGraphNode&);
    NodeGraphNode& operator=(const NodeGraphNode&);
};

}

#endif
