//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Author:  Bastian Damman
//Created: 23/11/2012

#ifndef _HE_MaterialGeneratorGraph_H_
#define _HE_MaterialGeneratorGraph_H_
#pragma once

#include "ITickable.h"
#include "IDrawable2D.h"
#include "MaterialGeneratorNodeInOut.h"
#include "NodeGraph.h"
#include "CommandStack.h"

#include "MaterialGeneratorGraphCommands.h"

#include "MaterialGeneratorNode.h"

namespace he {
namespace gfx {
    class Window;
    class View;
    class Renderer2D;
}
namespace gui {
    class Sprite;
    class BezierShape2D;
}
namespace ct {
    class ShaderGenerator;
}
namespace tools {
class MaterialGeneratorNode;

class MaterialGeneratorGraph : public ge::ITickable, public gfx::IDrawable2D
{
    friend class MaterialGeneratorGraphMoveCommand;
    friend class MaterialGeneratorGraphEditSelectionCommand;
    friend class MaterialGeneratorGraphNodeConnectCommand;
    friend class MaterialGeneratorGraphCreateCommand;
    friend class MaterialGeneratorGraphDeleteCommand;
    enum State
    {
        State_Idle,
        State_StartPan,
        State_Pan,
        State_StartMoveNode,
        State_MoveNode,
        State_ConnectNode
    };
    struct Shortcut
    {
        Shortcut(): m_Key(io::Key_MAX), m_Type(MaterialGeneratorNodeType_Unassigned) {}
        Shortcut(const io::Key key, const MaterialGeneratorNodeType type)
         : m_Key(key), m_Type(type) {}

        ~Shortcut() {}

        io::Key m_Key;
        MaterialGeneratorNodeType m_Type;
    };

public:

    /* CONSTRUCTOR - DESTRUCTOR */
    MaterialGeneratorGraph();
    virtual ~MaterialGeneratorGraph();

    /* GENERAL */
    void init();

    void open();
    void close();
    
    virtual void tick(float dTime);
    virtual void draw2D(gfx::Canvas2D* canvas);

    /* GETTERS */
    ct::ShaderGenerator* getGenerator() const { return m_Generator; }
    bool isOpen() const;

    MaterialGeneratorNode* getNode(const Guid& guid) const;
    MaterialGeneratorNode* getSelectedNode(const Guid& guid) const;

    void pushError(const MaterialGeneratorError& errorMsg);

private:

    /* INTERNAL */
    bool doNodeSelect(const vec2& mousePos, const bool keepSelection, const bool removeSelection);
    bool doConnectStart(const vec2& mousePos);
    bool doConnectEnd(const vec2& mousePos);

    vec2 screenToWorldPos(const vec2& screenPos) const;
    vec2 worldToScreenPos(const vec2& worldPos) const;

    void renderBackground();
    void onViewResized();

    void addNode(MaterialGeneratorNode* const node);

    /* MEMBERS */
    ct::ShaderGenerator* m_Generator;

    gui::Text m_DebugText;
    
    NodeGraph<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> m_NodeGraph;
    he::PrimitiveList<MaterialGeneratorNode*> m_NodeList;
    he::ObjectList<NodeGraphError<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput>> m_ErrorList;
    he::PrimitiveList<MaterialGeneratorNode*> m_SelectedNodeList;

    gfx::Window* m_Window;
    gfx::View* m_View;

    gfx::Renderer2D* m_Renderer;

    State m_State;
    vec2 m_GrabWorldPos;

    vec2 m_Offset;
    float m_Scale;
    bool m_IsActive;

    CommandStack m_CommandStack;
    MaterialGeneratorGraphMoveCommand m_MoveCommand;
    MaterialGeneratorGraphEditSelectionCommand m_EditSelectionCommand;
    MaterialGeneratorGraphNodeConnectCommand m_ConnectNodeCommand;
    MaterialGeneratorGraphCreateCommand m_CreateCommand;
    MaterialGeneratorGraphDeleteCommand m_DeleteCommand;

    he::ObjectList<Shortcut> m_ShortcutList;
    
    gui::Sprite* m_Background;

    gui::BezierShape2D* m_GhostConnection;

    struct ErrorMessage
    {
        float m_TimeLeft;
        gui::Text* m_Text;
        vec2 m_TextSize;
        vec2 m_Position;
    };
    void increaseErrorPool(const size_t extraSize);
    gfx::Font* m_ErrorFont;
    he::PrimitiveList<gui::Text*> m_ErrorPool;
    he::ObjectList<ErrorMessage> m_VisibleErrors;
    gui::Sprite* m_ErrorBackgroundSprite;

    /* DEFAULT COPY & ASSIGNMENT */
    MaterialGeneratorGraph(const MaterialGeneratorGraph&);
    MaterialGeneratorGraph& operator=(const MaterialGeneratorGraph&);
};

} } //end namespace

#endif
